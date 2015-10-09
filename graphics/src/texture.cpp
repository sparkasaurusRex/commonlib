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

Texture::Texture(const int w, const int h, const GLuint m)
{
  fname[0] = '\0';

  assert(w > 0 && h > 0);
  dim[0] = w;
  dim[1] = h;

  gl_mode = m;
}

Texture::Texture(const char *n)
{
  strcpy(fname, n);
}

Texture::~Texture()
{
  glDeleteTextures(1, &gl_texture);
}

void Texture::init()
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

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Filtering

  assert(glIsTexture(gl_texture) == GL_TRUE);
}

bool Texture::load()
{
    /*
    if(gl_texture != 0)
    {
        assert(glIsTexture(gl_texture) == GL_TRUE);
        cerr<<"texture already loaded???"<<endl;
        return true; //already loaded
    }*/

    int width, height, channels;
    //char fname[] = "/home/chandra/brick/game/data/textures/cf_wtr_drop01.tga";

#if defined(__USE_SOIL__)
    unsigned char *image = SOIL_load_image(fname, &width, &height, &channels, SOIL_LOAD_RGBA);
    assert(image);
    dim[0] = width;
    dim[1] = height;
#else
    //SDL2 way of loading
    cout<<"SDL Surface image load"<<endl;
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Filtering

#if defined(__USE_SOIL__)
    SOIL_free_image_data(image);
#else
    //SDL_UnlockSurface(image);
    SDL_FreeSurface(image);
#endif

  assert(glIsTexture(gl_texture) == GL_TRUE);

  return true;
}

bool Texture::load_from_file_data(TextureFileData &tfd)
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Filtering

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 dim[0],
                 dim[1],
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 tfd.image);

    return true;
}

//set up the texture for rendering
bool Texture::render_gl() const
{
  glEnable(GL_TEXTURE_2D);

  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glBindTexture(GL_TEXTURE_2D, gl_texture);

  return true;
}

bool Texture::update_pixels_from_mem(void *pixels)
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
