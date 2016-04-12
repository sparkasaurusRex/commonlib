#include <SDL2/SDL.h>

//http://www.libsdl.org/projects/SDL_image/
//#if defined(__APPLE__)
//#include <SDL2_image/SDL_image.h>
//#else
#include <SDL2/SDL_image.h>
//#endif

//#define __USE_SOIL__

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#if defined(__USE_SOIL__)
#include <SOIL/SOIL.h>
#endif

#include <iostream>

#include <assert.h>

#include "texture.h"

using namespace std;
using namespace Graphics;

Texture2D::Texture2D(const unsigned int w, const unsigned int h, const GLuint m)
{
  fname[0] = '\0';

  assert(w > 0 && h > 0);
  dim[0] = w; dim[1] = h;

  filter_mode = GL_LINEAR;
  wrap_mode[0] = GL_REPEAT;
  wrap_mode[1] = GL_REPEAT;
  gl_mode = m;
}

Texture2D::Texture2D(const char *n)
{
  strcpy(fname, n);
  filter_mode = GL_LINEAR;
  wrap_mode[0] = GL_REPEAT;
  wrap_mode[1] = GL_REPEAT;
  gl_mode = GL_RGBA;
}

Texture2D::~Texture2D()
{
  glDeleteTextures(1, &gl_texture);
}

void Texture2D::init()
{
  glGenTextures(1, &gl_texture);
  glBindTexture(GL_TEXTURE_2D, gl_texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  assert(glIsTexture(gl_texture) == GL_TRUE);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               gl_mode,
               dim[0],
               dim[1],
               0,
               gl_mode,
               GL_UNSIGNED_BYTE,
               NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mode);

  assert(glIsTexture(gl_texture) == GL_TRUE);
}

bool Texture2D::load()
{
    /*
    if(gl_texture != 0)
    {
        assert(glIsTexture(gl_texture) == GL_TRUE);
        cerr<<"texture already loaded???"<<endl;
        return true; //already loaded
    }*/

    int width, height, channels;

#if defined(__USE_SOIL__)
    unsigned char *image = SOIL_load_image(fname, &width, &height, &channels, SOIL_LOAD_RGBA);
    assert(image);
    dim[0] = width;
    dim[1] = height;
#else
    //SDL2 way of loading
    cout<<"IMG_Load("<<fname<<")"<<endl;
    SDL_Surface *image = IMG_Load(fname);

    assert(image);
    dim[0] = width = image->w;
    dim[1] = height = image->h;

    cout<<"image dimensions: "<<dim[0]<<", "<<dim[1]<<endl;

    if(image->format->BytesPerPixel == 3)
    {
      gl_mode = GL_RGB;
    }
    else if(image->format->BytesPerPixel == 4)
    {
      gl_mode = GL_RGBA;
    }

#endif

    glGenTextures(1, &gl_texture);
    glBindTexture(GL_TEXTURE_2D, gl_texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(glIsTexture(gl_texture) != GL_TRUE)
    {
      assert(false);
      return false;
    }

    glTexImage2D(GL_TEXTURE_2D,
                   0,
                   gl_mode,
                   width,
                   height,
                   0,
                   gl_mode,
                   GL_UNSIGNED_BYTE,
#if defined(__USE_SOIL__)
                 image);
#else
                 image->pixels);
#endif

    //not sure if these should go here, or in the render loop
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mode);

#if defined(__USE_SOIL__)
    SOIL_free_image_data(image);
#else
    //SDL_UnlockSurface(image);
    SDL_FreeSurface(image);
#endif

  assert(glIsTexture(gl_texture) == GL_TRUE);

  return true;
}

/*
bool Texture2D::load_from_file_data(TextureFileData &tfd)
{
    if(gl_texture != 0)
    {
      assert(glIsTexture(gl_texture) == GL_TRUE);
      return true; //already loaded
    }

    glGenTextures(1, &gl_texture);

    glBindTexture(GL_TEXTURE_2D, gl_texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(glIsTexture(gl_texture) != GL_TRUE)
  {
      assert(false);
      return false;
  }

    dim[0] = tfd.dim[0];
    dim[1] = tfd.dim[1];

    //not sure if these should go here, or in the render loop
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_mode)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mode);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 gl_mode,
                 dim[0],
                 dim[1],
                 0,
                 gl_mode,
                 GL_UNSIGNED_BYTE,
                 tfd.image);

    return true;
}
*/

//set up the texture for rendering
void Texture2D::render_gl(GLuint tex_stage) const
{
  //return true;
  glActiveTexture(tex_stage);
  //glClientActiveTexture(GL_TEXTURE0);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_3D);

  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glBindTexture(GL_TEXTURE_2D, gl_texture);
}

bool Texture2D::update_pixels_from_mem(void *pixels)
{
  int mip_level = 0;
  glBindTexture(GL_TEXTURE_2D, gl_texture);
  //glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, dim[0], dim[1]);
  glTexSubImage2D(GL_TEXTURE_2D,
                  mip_level,        //mip level to overwrite
                  0,                //starting u-coord
                  0,                //starting v-coord
                  dim[0],            //width of update rect
                  dim[1],            //height of update rect
                  gl_mode,          //pixel format
                  GL_UNSIGNED_BYTE,
                  pixels);          //pointer to pixel data
  return true;
}


Texture3D::Texture3D(const unsigned int w, const unsigned int h, const unsigned int d, const GLuint m)
{
  fname[0] = '\0';

  assert(w > 0 && h > 0 && d > 0);
  dim[0] = w; dim[1] = h; dim[2] = d;

  gl_mode = m;
  wrap_mode[0] = wrap_mode[1] = wrap_mode[2] = GL_REPEAT;
  filter_mode = GL_NEAREST;
}

Texture3D::Texture3D(const char *n)
{
  strcpy(fname, n);
  gl_mode = GL_RGBA;
  wrap_mode[0] = wrap_mode[1] = wrap_mode[2] = GL_REPEAT;
  filter_mode = GL_NEAREST;
}

Texture3D::~Texture3D()
{
  glDeleteTextures(1, &gl_texture);
}

void Texture3D::init()
{
  glGenTextures(1, &gl_texture);
  glBindTexture(GL_TEXTURE_3D, gl_texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  assert(glIsTexture(gl_texture) == GL_TRUE);

  glTexImage3D(GL_TEXTURE_3D,
               0,
               gl_mode,
               dim[0],
               dim[1],
               dim[2],
               0,
               gl_mode,
               GL_UNSIGNED_BYTE,
               NULL);

   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap_mode[0]);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap_mode[1]);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap_mode[2]);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter_mode);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter_mode);

   assert(glIsTexture(gl_texture) == GL_TRUE);
}

bool Texture3D::load(const unsigned int depth)
{
    cout<<"Texture3D::load()..."<<endl;
    cout<<"\t"<<fname<<endl;
    int width, height, channels;

#if defined(__USE_SOIL__)
    unsigned char *image = SOIL_load_image(fname, &width, &height, &channels, SOIL_LOAD_RGBA);
    assert(image);
    dim[0] = width / depth;
    dim[1] = height;
    dim[2] = depth;
#else
    //SDL2 way of loading
    cout<<"IMG_Load("<<fname<<")"<<endl;
    SDL_Surface *image = IMG_Load(fname);

    assert(image);
    dim[0] = width = image->w / depth;
    dim[1] = height = image->h;
    dim[2] = depth;

    cout<<"image dimensions: "<<dim[0]<<", "<<dim[1]<<endl;

    if(image->format->BytesPerPixel == 3)
    {
      gl_mode = GL_RGB;
    }
    else if(image->format->BytesPerPixel == 4)
    {
      gl_mode = GL_RGBA;
    }

#endif

    glGenTextures(1, &gl_texture);
    glBindTexture(GL_TEXTURE_3D, gl_texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(glIsTexture(gl_texture) != GL_TRUE)
    {
      assert(false);
      return false;
    }

    glTexImage3D(GL_TEXTURE_3D,
                 0,
                 gl_mode,
                 dim[0],
                 dim[1],
                 dim[2],
                 0,
                 gl_mode,
                 GL_UNSIGNED_BYTE,
#if defined(__USE_SOIL__)
                 image);
#else
                 image->pixels);
#endif

    //not sure if these should go here, or in the render loop
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap_mode[0]);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap_mode[1]);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap_mode[2]);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter_mode); // Linear Filtering
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter_mode); // Linear Filtering

#if defined(__USE_SOIL__)
    SOIL_free_image_data(image);
#else
    SDL_FreeSurface(image);
#endif

  assert(glIsTexture(gl_texture) == GL_TRUE);

  return true;
}

//set up the texture for rendering
bool Texture3D::render_gl(GLuint tex_stage) const
{
  //return true;
  glActiveTexture(tex_stage);
  //glClientActiveTexture(GL_TEXTURE0);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_3D);
  glDisable(GL_TEXTURE_2D);

  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  //What does this do, really?
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glBindTexture(GL_TEXTURE_3D, gl_texture);

  return true;
}
