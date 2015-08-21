#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <SDL2/SDL.h>
#include <iostream>
#include <assert.h>

#include "texture.h"
#include "perlin.h"
#include "fluid2d.h"
#include "font.h"

using namespace std;

const int WIN_WIDTH = 512;
const int WIN_HEIGHT = 512;

SDL_Window* sdl_window = NULL;
SDL_GLContext sdl_gl_context = NULL;
Texture *t = NULL;
Texture *dynamic_tex = NULL;
Texture *fluid_tex = NULL;

Font *font = NULL;

Fluid2D *fluid = NULL;

void quit_app()
{
  SDL_GL_DeleteContext(sdl_gl_context);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
  exit(0);
}

void init_sdl()
{
	if(SDL_Init( SDL_INIT_VIDEO ) < 0)
  {
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
  }
  else
  {
    //Create window
    sdl_window = SDL_CreateWindow("Graphics Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    assert(sdl_window);

    sdl_gl_context = SDL_GL_CreateContext(sdl_window);
    assert(sdl_gl_context);

    SDL_GL_SetSwapInterval(1);
  }
}

void init_gl()
{

}

void process_events()
{
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);
  if(keystate[SDLK_ESCAPE]) quit_app();

  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    if(event.type == SDL_QUIT)
    {
      quit_app();
    }
  }
}

//fill our dynamic texture with perlin noise
void fill_dynamic_texture(float t)
{
  int w, h;
  dynamic_tex->get_dim(w, h);
  GLuint mode = dynamic_tex->get_pixel_mode();
  int num_bytes = (mode == GL_RGBA) ? 4 : 3;

  //not exactly optimal to allocate / deallocate this every frame...
  GLubyte *pixels = new GLubyte[w * h * num_bytes];

  for(int i = 0; i < w; i++)
  {
    for(int j = 0; j < h; j++)
    {
      float x_val = 1.0f * (float)i / (float)w;
      float y_val = 1.0f * (float)j / (float)h;
      float z_val = t * 0.0002f;
      float val = 0.5f * PerlinNoise::octave_noise_3d(2, 1.0f, 4.0f, x_val, y_val, z_val) + 0.5f;
      //float val = PerlinNoise::marble_noise_3d(2, 1.0f, 5.0f, x_val, y_val, z_val);
      //float val = PerlinNoise::raw_noise_3d(x_val, y_val, z_val);
      //float val = 0.5f * sin(M_PI * 2.0f * (float)i / (float)w) + 0.5f;
      for(int oct = 0; oct < 3; oct++)
      {
        pixels[((i * w + j) * num_bytes) + oct] = (GLubyte)(val * 255.0f);
      }
      if(num_bytes == 4)
      {
        pixels[((i * w + j) * 4) + 3] = (GLubyte)255.0f;
      }
    }
  }

  //cout<<"updating pixels..."<<endl;
  dynamic_tex->update_pixels_from_mem(pixels);
  delete pixels;
}

void fill_fluid_texture(float t)
{
  //return;
  fluid->simulate(t);
  const float *f = fluid->get_density_array();

  int w, h;
  fluid_tex->get_dim(w, h);
  GLuint mode = fluid_tex->get_pixel_mode();
  int num_bytes = (mode == GL_RGBA) ? 4 : 3;

  GLubyte *pixels = new GLubyte[w * h * num_bytes];
  for(int i = 0; i < w; i++)
  {
    for(int j = 0; j < h; j++)
    {
      int fluid_idx = i + (w + 2) * j;
      float val = f[fluid_idx];

      for(int oct = 0; oct < 3; oct++)
      {
        pixels[((i * w + j) * num_bytes) + oct] = (GLubyte)(val * 255.0f);
      }
      if(num_bytes == 4)
      {
        pixels[((i * w + j) * 4) + 3] = (GLubyte)255.0f;
      }
    }
  }

  fluid_tex->update_pixels_from_mem(pixels);
  delete pixels;
}

void render_font_test()
{
  glColor3f(1.0f, 0.0f, 0.0f);
  font->print(270.0f, 220.0f, "The quick brown fox\njumps over the\nlazy dog!?");
}

void game_loop()
{
  Uint32 ticks = SDL_GetTicks();
  float game_time = (float)ticks;

  fill_dynamic_texture(game_time);
  fill_fluid_texture(game_time);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //texture from file
  t->render_gl();
  glBegin(GL_QUADS);
  	glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
  glEnd();

  //render dynamic texture
  dynamic_tex->render_gl();
  glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
  glEnd();

  //render fluid texture
  fluid_tex->render_gl();
  glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);
  glEnd();

  render_font_test();

	glFlush();
	SDL_GL_SwapWindow(sdl_window);
}

int main(int argc, char **argv)
{
	init_sdl();
	init_gl();

  t = new Texture("data/test.tga");
  t->load();

  font = new Font("/Library/Fonts/AmericanTypewriter.ttc", 16);
  font->init();

  dynamic_tex = new Texture(256, 256, GL_RGB);
  dynamic_tex->init();

  fluid_tex = new Texture(256, 256, GL_RGB);
  fluid_tex->init();

  fluid = new Fluid2D(256, 256);

  while(true)
  {
    process_events();
    game_loop();
  }

  quit_app();

  delete fluid;

  delete t;
  delete dynamic_tex;
  delete fluid_tex;

	return 0;
}
