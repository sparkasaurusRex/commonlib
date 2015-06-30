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

using namespace std;

const int WIN_WIDTH =   512;
const int WIN_HEIGHT =  512;

float Previous_game_time = 0.0f;

SDL_Window* sdl_window =        NULL;
SDL_GLContext sdl_gl_context =  NULL;
Texture *fluid_tex =            NULL;

Fluid2D *fluid =                NULL;

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
      switch(event.type)
      {
        case SDL_MOUSEBUTTONDOWN:
        //case SDL_MOUSEMOTION:
        {
          int w, h;
          fluid_tex->get_dim(w, h);
          //cout<<"click ("<<event.button.x<<", "<<event.button.y<<")"<<endl;
          Float2 click_pt((float)event.button.x / WIN_WIDTH, (float)event.button.y / WIN_HEIGHT);
          fluid->add_density_at_point(click_pt, 10.0f, 0.03f);
          break;
        }
        case SDL_MOUSEMOTION:
        {
          if(event.motion.state & SDL_BUTTON(1))
          {
            //cout<<"button 1 pressed and mouse moving"<<endl;
            Float2 pt((float)event.motion.x / WIN_WIDTH, (float)event.motion.y / WIN_HEIGHT);
            Float2 vel((float)event.motion.xrel, (float)event.motion.yrel);
            fluid->add_velocity_at_point(pt, vel, 0.1f);
          }
          break;
        }
        case SDL_QUIT:
          quit_app();
          break;
      }
    }
}

void fill_fluid_texture(float t)
{
  fluid->simulate(t * 0.01f);
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
        float val = min(f[fluid_idx], 1.0f);
        //cout<<val<<endl;

        for(int oct = 0; oct < 3; oct++)
        {
          pixels[((i * w + j) * num_bytes) + oct] = (GLubyte)(val * 255.0f);
        }
        if(num_bytes == 4)
        {
          pixels[((i * w + j) * 4) + 3] = (GLubyte)(val * 255.0f);
        }
    }
  }

  fluid_tex->update_pixels_from_mem(pixels);
  delete pixels;
}

void game_loop()
{
    Uint32 ticks = SDL_GetTicks();
    float game_time = (float)ticks;
    float dt = (game_time - Previous_game_time) * 0.01f;
    //cout<<dt<<endl;
    Previous_game_time = game_time;

    //fill_dynamic_texture(game_time);
    fill_fluid_texture(dt);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //render fluid texture
    fluid_tex->render_gl();
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();

	glFlush();
	SDL_GL_SwapWindow(sdl_window);
}

int main(int argc, char **argv)
{
	init_sdl();
	init_gl();

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
  delete fluid_tex;

	return 0;
}
