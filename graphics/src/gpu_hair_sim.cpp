#include <stddef.h>
#include <assert.h>
#include "gpu_hair_sim.h"
#include "perlin.h"

using namespace Graphics;

GPUHairSim::GPUHairSim()
{
  num_hairs = 4;
  num_segments = 4;

  pos_tex[0] = pos_tex[1] = 0;
  vbo = ibo = 0;
  verts = NULL;
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    assert(glIsTexture(pos_tex[i]) == GL_TRUE);

    //fill textures w/ initial values
    int num_bytes = (pixel_mode == GL_RGBA) ? 4 : 3;
    GLubyte *pixels = new GLubyte[num_hairs * num_segments * num_bytes];

    for(int i = 0; i < num_hairs; i++)
    {
      for(int j = 0; j < num_segments; j++)
      {
        float x_val = 1.0f * (float)i / (float)num_hairs;
        float y_val = 1.0f * (float)j / (float)num_segments;
        float z_val = 0.0f;
        float val = 0.5f * PerlinNoise::octave_noise_3d(2, 1.0f, 4.0f, x_val, y_val, z_val) + 0.5f;

        for(int oct = 0; oct < 3; oct++)
        {
          pixels[((i * num_hairs + j) * num_bytes) + oct] = (GLubyte)(val * 255.0f);
        }
        if(num_bytes == 4)
        {
          pixels[((i * num_hairs + j) * 4) + 3] = (GLubyte)255.0f;
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

}

void GPUHairSim::deinit()
{
  glDeleteTextures(2, pos_tex);

  //tear down vbo, ibo
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  if(verts) { delete verts; }

  pos_tex[0] = pos_tex[1] = vbo = ibo = 0;
  verts = NULL;
}

void GPUHairSim::simulate(const float dt)
{
  //glTexSubImage2D - to update forces
  //render pos + force -> new pos texture
}

void GPUHairSim::render()
{

}
