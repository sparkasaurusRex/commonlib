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
  force_tex_dim[0] = force_tex_dim[1] = 256;

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
  glBindTexture(GL_TEXTURE_2D, force_tex);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  assert(glIsTexture(force_tex) == GL_TRUE);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA16F_ARB,
               force_tex_dim[0],
               force_tex_dim[1],
               0,
               GL_RGB,
               GL_FLOAT,
               NULL);
  /*glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/

  Float3 *hair_pos = new Float3[num_hairs];
  for(int i = 0; i < num_hairs; i++)
  {
    hair_pos[i] = Float3(random(-1.0f, 1.0f), 0.0f, random(-1.0f, 1.0f));
  }

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

    //pixel_mode = GL_RGBA_FLOAT32_ATI;//GL_RGBA16F_ARB;

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA16F_ARB,
                 num_hairs,           //u axis = hair index
                 num_segments,        //v axis = hair segment index
                 0,
                 pixel_mode, //GL_RGBA16F_ARB,      //RGB = xyz (pos), A = ?
                 GL_FLOAT,//GL_UNSIGNED_BYTE,
                 NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    assert(glIsTexture(pos_tex[i]) == GL_TRUE);

    //fill textures w/ initial values (random points in [-1, 1], [-1, 1])
    int pixel_idx = 0;
    float height = 1.0f;
    int num_bytes = (pixel_mode == GL_RGBA) ? 4 : 3;
    GLfloat *pixels = new GLfloat[num_hairs * num_segments * num_bytes];
    for(int k = 0; k < num_segments; k++)
    {
      float seg_pct = (float)k / (float)num_segments;
      for(int j = 0; j < num_hairs; j++)
      {
        /*Float3 root_pos((float)j / (float)num_hairs, seg_pct, 0.0f);
        pixels[pixel_idx++] = (GLubyte)(root_pos[0] * 255.0f);
        pixels[pixel_idx++] = (GLubyte)(root_pos[1] * 255.0f);
        pixels[pixel_idx++] = (GLubyte)(root_pos[2] * 255.0f);
        */
        pixels[pixel_idx++] = hair_pos[j][0];
        pixels[pixel_idx++] = height * seg_pct;
        pixels[pixel_idx++] = hair_pos[j][2];
        if(num_bytes == 4)
        {
          pixels[pixel_idx++] = 1.0f;
        }
      }
    }

    /*for(int j = 0; j < num_hairs; j++)
    {
      Float3 root_pos(0.0f, 0.0f, (float)j / (float)num_hairs);
      for(int k = 0; k < num_segments; k++)
      {
        float x_val = 1.0f * (float)j / (float)num_hairs;
        float y_val = 1.0f * (float)k / (float)num_segments;
        float z_val = 0.0f;
        float val = 0.0f;//0.5f * PerlinNoise::octave_noise_3d(2, 1.0f, 4.0f, x_val, y_val, z_val) + 0.5f;

        for(int oct = 0; oct < 3; oct++)
        {
          pixels[((j * num_segments + k) * num_bytes) + oct] = (GLubyte)(root_pos[oct] * 255.0f);
        }
        if(num_bytes == 4)
        {
          pixels[((j * num_segments + k) * 4) + 3] = (GLubyte)255.0f;
        }
      }
    }*/

    glTexSubImage2D(GL_TEXTURE_2D,
                    0,                //mip level
                    0,                //starting u coord
                    0,                //starting v coord
                    num_hairs,        //width of update rect
                    num_segments,     //height of update rect
                    pixel_mode,       //pixel format
                    GL_FLOAT,
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
  }

  //create our geometry buffers
  num_verts = num_hairs * (num_segments) * 2;
  num_indices = num_verts;
  verts = new HairVert[num_verts];
  indices = new unsigned int[num_indices];

  int v_idx = 0; int i_idx = 0;

  for(int i = 0; i < num_hairs; i++)
  {
    float height = 1.0f;

    Float3 col_a(0.1f, 0.1f, 0.05f);
    Float3 col_b(0.15f, 0.7f, 0.35f);


    for(int j = 1; j < num_segments + 1; j++)
    {
      for(int k = j - 1; k <= j; k++)
      {
        float segment_pct = (float)(k) / (float)num_segments;
        verts[v_idx].x = 0.0f;
        verts[v_idx].y = 0.0f;//height * segment_pct;
        verts[v_idx].z = 0.0f;
        verts[v_idx].r = lerp(col_a[0], col_b[0], segment_pct);
        verts[v_idx].g = lerp(col_a[1], col_b[1], segment_pct);
        verts[v_idx].b = lerp(col_a[2], col_b[2], segment_pct);
        verts[v_idx].u0 = (float)i / (float)num_hairs;
        verts[v_idx].v0 = segment_pct;

        indices[i_idx++] = v_idx++;
      }
    }
  }

  delete hair_pos;

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
  float speed = 1.0f;
  float scale = 1.0f;

  //update the force texture
  glBindTexture(GL_TEXTURE_2D, force_tex);
  int num_bytes = 3;
  GLfloat *pixels = new GLfloat[force_tex_dim[0] *
                                force_tex_dim[1] *
                                num_bytes];


  int pix_idx = 0;
  for(int i = 0; i < force_tex_dim[0]; i++)
  {
    for(int j = 0; j < force_tex_dim[1]; j++)
    {
      float x = (float)i / (float)force_tex_dim[0];
      float y = (float)j / (float)force_tex_dim[1];
      pixels[pix_idx++] = scaled_octave_noise_3d(2, 1.0f, scale, -1.0f, 1.0f, x, y, game_time * speed);
      pixels[pix_idx++] = 0;//(GLubyte)(scaled_octave_noise_4d(2, 1.0f, scale * 0.9f, -255.0f, 255.0f, x + 12.43f, y + 9.13f, z + 4.13f, game_time * speed));
      pixels[pix_idx++] = scaled_octave_noise_3d(2, 1.0f, scale * 1.2f, -1.0f, 1.0f, x + 3.12f, y + 67.12f, game_time * speed);
    }
  }

  glTexSubImage2D(GL_TEXTURE_2D,
                  0,                //mip level
                  0,                //starting u coord
                  0,                //starting v coord
                  force_tex_dim[0],     //width of update rect
                  force_tex_dim[1],     //height of update rect
                  GL_RGB,               //pixel format
                  GL_FLOAT,
                  pixels);          //pointer to pixel data

  delete pixels;

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

  //prev pos tex (1)
  GLuint prev_tex_loc = glGetUniformLocation(shader->gl_shader_program, "prev_pos_tex");
  glUniform1i(prev_tex_loc, 0);
  glActiveTexture(GL_TEXTURE0);
  glClientActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pos_tex[1]);

  //force tex
  GLuint force_tex_loc = glGetUniformLocation(shader->gl_shader_program, "force_tex");
  glUniform1i(force_tex_loc, 1);
  glActiveTexture(GL_TEXTURE1);
  //glClientActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, force_tex);

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
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_3D);

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

void GPUHairSim::render()
{
  glLineWidth(1.0f);

  render_mat.render_gl();

  Shader *shader = render_mat.get_shader();

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  //glDepthRange(0.0f, 1.0f);

  //set up uniforms
  GLint uloc = glGetUniformLocation(shader->gl_shader_program, "hair_tex");
  glUniform1i(uloc, 0);
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
