#include <stddef.h>
#include <assert.h>
#include "gpu_hair_sim.h"
#include "math_utility.h"
#include "perlin.h"

using namespace Math;
using namespace Graphics;

GPUHairSim::GPUHairSim()
{
  num_hairs = 4;
  num_segments = 4;

  pos_tex[0] = pos_tex[1] = 0;
  vbo = ibo = 0;
  verts = NULL;
  num_verts = 0;
  indices = NULL;
  num_indices = 0;
}

GPUHairSim::~GPUHairSim()
{
  deinit();
}

void GPUHairSim::init()
{
  //create our textures
  glGenTextures(2, pos_tex);
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
        float val = 0.5f * PerlinNoise::octave_noise_3d(2, 1.0f, 4.0f, x_val, y_val, z_val) + 0.5f;

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

    float height = 0.2f;

    Float3 col_a(0.1f, 0.1f, 0.05f);
    Float3 col_b(0.1f, 1.0f, 0.3f);

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

void GPUHairSim::simulate(const float dt)
{
  //TODO
  //glTexSubImage2D - to update forces
  //set uniform variable (dt)
  //render pos + force -> new pos texture
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
