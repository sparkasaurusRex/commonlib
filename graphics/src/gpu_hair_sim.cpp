#include <stddef.h>
#include <assert.h>
#include <iostream>
#include "gpu_hair_sim.h"
#include "math_utility.h"
#include "perlin.h"

using namespace std;
using namespace Math;
using namespace Graphics;
using namespace PerlinNoise;

GPUHairSim::GPUHairSim()
{
  num_hairs = 4;
  num_segments = 4;

  pos_fbo[0] = pos_fbo[1] = 0;
  pos_tex[0] = pos_tex[1] = 0;
  force_tex = 0;
  vbo = ibo = 0;
  verts = NULL;
  num_verts = 0;
  indices = NULL;
  num_indices = 0;
  force_tex_dim[0] = force_tex_dim[1] = force_tex_dim[2] = 32;

  fbo_indices[0] = 0;
  fbo_indices[1] = 1;
  fbo_indices[2] = 2;
  fbo_indices[3] = 3;

  fbo_verts[0].x =  -1.0f;
  fbo_verts[0].y =  -1.0f;
  fbo_verts[0].z =   0.0f;
  fbo_verts[0].u =   0.0f;
  fbo_verts[0].v =   0.0f;

  fbo_verts[1].x =   1.0f;
  fbo_verts[1].y =  -1.0f;
  fbo_verts[1].z =   0.0f;
  fbo_verts[1].u =   1.0f;
  fbo_verts[1].v =   0.0f;

  fbo_verts[2].x =   1.0f;
  fbo_verts[2].y =   1.0f;
  fbo_verts[2].z =   0.0f;
  fbo_verts[2].u =   1.0f;
  fbo_verts[2].v =   1.0f;

  fbo_verts[3].x =  -1.0f;
  fbo_verts[3].y =   1.0f;
  fbo_verts[3].z =   0.0f;
  fbo_verts[3].u =   0.0f;
  fbo_verts[3].v =   1.0f;

  num_fbo_verts = num_fbo_indices = 4;
  fbo_vbo = fbo_ibo = 0;
}

GPUHairSim::~GPUHairSim()
{
  deinit();
}

void GPUHairSim::init()
{
  //1. create our textures

  //create a 3D force texture
  glGenTextures(1, &force_tex);
  glBindTexture(GL_TEXTURE_3D, force_tex);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  assert(glIsTexture(force_tex) == GL_TRUE);
  glTexImage3D(GL_TEXTURE_3D,
               0,
               GL_RGB,
               force_tex_dim[0],
               force_tex_dim[1],
               force_tex_dim[2],
               0,
               GL_RGB,
               GL_UNSIGNED_BYTE,
               NULL);
  /*glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/

  //create 2 2D displacement textures to represet current and previous
  //frame displacement per hair
  glGenTextures(2, pos_tex);
  glGenFramebuffersEXT(2, pos_fbo);
  for(int i = 0; i < 2; i++)
  {
    GLuint pixel_mode = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, pos_tex[i]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    assert(glIsTexture(pos_tex[i]) == GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 pixel_mode,
                 num_hairs,           //u axis = hair index
                 num_segments,        //v axis = hair segment index
                 0,
                 pixel_mode,          //RGB = xyz (pos), A = ?
                 GL_UNSIGNED_BYTE,
                 NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    assert(glIsTexture(pos_tex[i]) == GL_TRUE);

    //fill textures w/ initial values
    int num_bytes = (pixel_mode == GL_RGBA) ? 4 : 3;
    GLubyte *pixels = new GLubyte[num_hairs * num_segments * num_bytes];

    for(int j = 0; j < num_hairs; j++)
    {
      for(int k = 0; k < num_segments; k++)
      {
        float x_val = 1.0f * (float)j / (float)num_hairs;
        float y_val = 1.0f * (float)k / (float)num_segments;
        float z_val = 0.0f;
        float val = 0.0f;//0.5f * PerlinNoise::octave_noise_3d(2, 1.0f, 4.0f, x_val, y_val, z_val) + 0.5f;

        for(int oct = 0; oct < 3; oct++)
        {
          pixels[((j * num_segments + k) * num_bytes) + oct] = (GLubyte)(val * 255.0f);
        }
        if(num_bytes == 4)
        {
          pixels[((j * num_segments + k) * 4) + 3] = (GLubyte)255.0f;
        }
      }
    }

    glTexSubImage2D(GL_TEXTURE_2D,
                    0,                //mip level
                    0,                //starting u coord
                    0,                //starting v coord
                    num_hairs,        //width of update rect
                    num_segments,     //height of update rect
                    pixel_mode,       //pixel format
                    GL_UNSIGNED_BYTE,
                    pixels);          //pointer to pixel data

    delete pixels;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pos_fbo[i]);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, pos_tex[i], 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    //create FBOs + buffers for full screen quad
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pos_fbo[i]);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, pos_tex[i], 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    glGenBuffers(1, &fbo_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, fbo_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_verts), fbo_verts, GL_STATIC_DRAW);

    glGenBuffers(1, &fbo_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 4, fbo_indices, GL_STATIC_DRAW);

    //init render surfaces
    /*pos[i].set_fbo_res(num_hairs, num_segments);
    pos[i].set_internal_format(GL_RGBA);
    pos[i].set_format(GL_RGBA);
    pos[i].set_filtering_mode(GL_NEAREST);
    pos[i].set_shader_names(std::string("data/shaders/hair_sim.vs"), std::string("data/shaders/hair_sim.fs"));
    pos[i].init();*/
  }

  //create our geometry buffers
  num_verts = num_hairs * (num_segments) * 2;
  num_indices = num_verts;
  verts = new HairVert[num_verts];
  indices = new unsigned int[num_indices];

  int v_idx = 0; int i_idx = 0;

  //let's make a spiral for now
  Float3 root_pos(0.0f, 0.0f, 0.0f);
  float angle_offset = 0.2f;
  for(int i = 0; i < num_hairs; i++)
  {
    Float3 new_pos = root_pos + Float3(0.1f, 0.0f, 0.0f);
    float s = sin(angle_offset);
    float c = cos(angle_offset);

    root_pos[0] = new_pos[0] * c - new_pos[2] * s;
    root_pos[2] = new_pos[0] * s + new_pos[2] * c;

    float height = 0.5f;

    Float3 col_a(0.1f, 0.1f, 0.05f);
    Float3 col_b(0.15f, 0.7f, 0.35f);

    root_pos = Float3(random(-1.0f, 1.0f), 0.0f, random(-1.0f, 1.0f));

    for(int j = 1; j < num_segments + 1; j++)
    {
      for(int k = j - 1; k <= j; k++)
      {
        float segment_pct = (float)(k) / (float)num_segments;
        verts[v_idx].x = root_pos[0];
        verts[v_idx].y = height * segment_pct;
        verts[v_idx].z = root_pos[2];
        verts[v_idx].r = lerp(col_a[0], col_b[0], segment_pct);
        verts[v_idx].g = lerp(col_a[1], col_b[1], segment_pct);
        verts[v_idx].b = lerp(col_a[2], col_b[2], segment_pct);
        verts[v_idx].u = segment_pct;
        verts[v_idx].v = 0.0f;

        indices[i_idx++] = v_idx++;
      }
    }
  }

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(HairVert) * num_verts, &verts[0].x, GL_STATIC_DRAW);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, indices, GL_STATIC_DRAW);

  //load textures and shaders
  render_mat.set_shader_filenames(std::string("data/shaders/hair_render.vs"), std::string("data/shaders/hair_render.fs"));
  render_mat.init();

  sim_mat.set_shader_filenames(std::string("data/shaders/hair_sim.vs"), std::string("data/shaders/hair_sim.fs"));
  sim_mat.init();
}

void GPUHairSim::deinit()
{
  glDeleteTextures(2, pos_tex);
  glDeleteTextures(1, &force_tex);

  //tear down vbo, ibo
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  if(verts) { delete verts; }
  if(indices) { delete indices; }

  pos_tex[0] = pos_tex[1] = force_tex = vbo = ibo = 0;
  verts = NULL; indices = NULL;
  num_verts = num_indices = 0;
}

void GPUHairSim::simulate(const float game_time, const float dt)
{
  float speed = 0.1f;
  float scale = 1.0f;
  //TODO
  //glTexSubImage3D - to update forces
  //set uniform variable (dt)
  //render pos + force -> new pos texture

  if(true)
  {
    //update the force texture
    glBindTexture(GL_TEXTURE_3D, force_tex);
    int num_bytes = 3;
    GLubyte *pixels = new GLubyte[force_tex_dim[0] *
                                  force_tex_dim[1] *
                                  force_tex_dim[2] *
                                  num_bytes];


    int pix_idx = 0;
    for(int i = 0; i < force_tex_dim[0]; i++)
    {
      for(int j = 0; j < force_tex_dim[1]; j++)
      {
        for(int k = 0; k < force_tex_dim[2]; k++)
        {
          float x = (float)i / (float)force_tex_dim[0];
          float y = (float)j / (float)force_tex_dim[1];
          float z = (float)k / (float)force_tex_dim[2];
          pixels[pix_idx++] = (GLubyte)(scaled_octave_noise_4d(2, 1.0f, scale, -255.0f, 255.0f, x, y, z, game_time * speed));
          pixels[pix_idx++] = (GLubyte)(scaled_octave_noise_4d(2, 1.0f, scale * 0.9f, -255.0f, 255.0f, x + 12.43f, y + 9.13f, z + 4.13f, game_time * speed));
          pixels[pix_idx++] = (GLubyte)(scaled_octave_noise_4d(2, 1.0f, scale * 1.2f, -255.0f, 255.0f, x + 3.12f, y + 67.12f, z + 7.915f, game_time * speed));
        }
      }
    }

    glTexSubImage3D(GL_TEXTURE_3D,
                    0,                //mip level
                    0,                //starting u coord
                    0,                //starting v coord
                    0,
                    force_tex_dim[0],     //width of update rect
                    force_tex_dim[1],     //height of update rect
                    force_tex_dim[2],     //depth
                    GL_RGB,               //pixel format
                    GL_UNSIGNED_BYTE,
                    pixels);          //pointer to pixel data

    delete pixels;
    glBindTexture(GL_TEXTURE_3D, 0);

    //set the render target to the "current" position texture (0)
    GLint win_viewport[4];
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pos_fbo[0]);
    glGetIntegerv(GL_VIEWPORT, win_viewport);
    glViewport(0, 0, num_hairs, num_segments);

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    //render the HDR render surface to a full-screen quad
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    sim_mat.render_gl();
    Shader *shader = sim_mat.get_shader();

    //set uniforms + textures
    //dt
    GLuint dt_loc = glGetUniformLocation(shader->gl_shader_program, "dt");
    glUniform1f(dt_loc, game_time * 0.001f);//dt);

    //force tex
    //prev pos tex (1)
    GLuint prev_tex_loc = glGetUniformLocation(shader->gl_shader_program, "prev_pos_tex");
    glUniform1i(prev_tex_loc, 0);
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pos_tex[1]);

    //draw calls
    glBindBuffer(GL_ARRAY_BUFFER, fbo_vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(FBOVert), (void *)0);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(FBOVert), (void *)(sizeof(float) * 3));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo_ibo);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (void *)0);

    //reset shader
    glUseProgramObjectARB(0);

    //release fbo
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glViewport(win_viewport[0], win_viewport[1], win_viewport[2], win_viewport[3]);

    //swap curr / prev textures
    GLuint temp = pos_tex[0];
    pos_tex[0] = pos_tex[1];
    pos_tex[1] = temp;
    temp = pos_fbo[0];
    pos_fbo[0] = pos_fbo[1];
    pos_fbo[1] = temp;
  }

  //TEMP
  /*
  glBindTexture(GL_TEXTURE_2D, pos_tex[0]);
  GLuint pixel_mode = GL_RGBA;
  int num_bytes = (pixel_mode == GL_RGBA) ? 4 : 3;
  GLubyte *pixels = new GLubyte[num_hairs * num_segments * num_bytes];

  for(int j = 0; j < num_hairs; j++)
  {
    for(int k = 0; k < num_segments; k++)
    {
      float x_val = 1.0f * (float)j / (float)num_hairs;
      float y_val = game_time * 0.1f;
      float z_val = 1.0f * (float)k / (float)num_segments;

      float val = 0.5f * PerlinNoise::octave_noise_3d(2, 1.0f, 1.0f, x_val, y_val, z_val) + 0.5f;

      for(int oct = 0; oct < 3; oct++)
      {
        pixels[((j * num_segments + k) * num_bytes) + oct] = (GLubyte)(val * 255.0f);
      }
      if(num_bytes == 4)
      {
        pixels[((j * num_segments + k) * 4) + 3] = (GLubyte)255.0f;
      }
    }
  }

  glTexSubImage2D(GL_TEXTURE_2D,
                  0,                //mip level
                  0,                //starting u coord
                  0,                //starting v coord
                  num_hairs,        //width of update rect
                  num_segments,     //height of update rect
                  pixel_mode,       //pixel format
                  GL_UNSIGNED_BYTE,
                  pixels);          //pointer to pixel data

  delete pixels;
  */
}

void GPUHairSim::render()
{
  glLineWidth(1.0f);

  render_mat.render_gl();

  Shader *shader = render_mat.get_shader();

  //set up uniforms
  GLint uloc = glGetUniformLocation(shader->gl_shader_program, "hair_tex");
  glUniform1i(uloc, 0);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  //glDepthRange(0.0f, 1.0f);

  glActiveTexture(GL_TEXTURE0);
  glClientActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pos_tex[0]);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(HairVert), (void *)0);

  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(3, GL_FLOAT, sizeof(HairVert), (void *)(sizeof(float) * 3));

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(HairVert), (void *)(sizeof(float) * 6));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_LINES, num_indices, GL_UNSIGNED_INT, (void *)0);

  //reset shader
  glUseProgramObjectARB(0);
}
