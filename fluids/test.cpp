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
#include "fluid2d_inflow.h"
#include "fluid2d_turbulence.h"
#include "fluid2d_turbulence_inflow.h"

using namespace std;

int WIN_WIDTH =   512;
int WIN_HEIGHT =  512;

int FLUID_DIM = 256;

float Previous_game_time = 0.0f;
float Time_scale = 0.0001f;
float Velocity_scale = 50.0f;
float Fluid_add_amount = 100.0f;

int Fluid_channel_display = 2;

SDL_Window* sdl_window =        NULL;
SDL_GLContext sdl_gl_context =  NULL;
Texture *fluid_tex =            NULL;

Fluid2D *fluid =                NULL;
Fluid2DInflow *inflow =         NULL;
Fluid2DTurbulenceField *turb =  NULL;
Fluid2DTurbulenceInflow *turb_in = NULL;
Fluid2DTurbulenceInflow *turb_out = NULL;


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
      printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
      //Create window
      sdl_window = SDL_CreateWindow("Fluid Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

    int mouse_x, mouse_y, num_keys;
    Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    const Uint8 *keyboard_state = SDL_GetKeyboardState(&num_keys);
    if(button_state & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
      float fluid_amt =  Fluid_add_amount;
      if(keyboard_state[SDL_SCANCODE_LCTRL])
      {
        fluid_amt = -fluid_amt;
      }
      int w, h;
      fluid_tex->get_dim(w, h);
      Float2 click_pt((float)mouse_x / WIN_WIDTH, (float)mouse_y / WIN_HEIGHT);
      //fluid->add_density_at_point(click_pt, fluid_amt, 0.025f);
    }

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_KEYDOWN:
        {
          if(keyboard_state[SDL_SCANCODE_D])
          {
            float diff_rate = fluid->get_diffusion_rate();
            if(keyboard_state[SDL_SCANCODE_UP])
            {
              diff_rate *= 2.0f;
            } else if(keyboard_state[SDL_SCANCODE_DOWN])
            {
              diff_rate *= 0.5f;
            }
            fluid->set_diffusion_rate(diff_rate);
            cout<<"diffusion rate: "<<diff_rate<<endl;
          }
          if(keyboard_state[SDL_SCANCODE_V])
          {
            float viscosity = fluid->get_viscosity();
            if(keyboard_state[SDL_SCANCODE_UP])
            {
              viscosity *= 2.0f;
            }
            else if(keyboard_state[SDL_SCANCODE_DOWN])
            {
              viscosity *= 0.5f;
            }
            fluid->set_viscosity(viscosity);
            cout<<"viscosity: "<<viscosity<<endl;
          }
          if(keyboard_state[SDL_SCANCODE_T])
          {
              if(keyboard_state[SDL_SCANCODE_UP])
              {
                Time_scale *= 2.0f;
              } else if(keyboard_state[SDL_SCANCODE_DOWN])
              {
                Time_scale *= 0.5f;
              }
              cout<<"time scale: "<<Time_scale<<endl;
          }
          if(keyboard_state[SDL_SCANCODE_P])
          {
            int project_steps = fluid->get_project_steps();
            if(keyboard_state[SDL_SCANCODE_UP])
            {
              project_steps++;
            } else if(keyboard_state[SDL_SCANCODE_DOWN]) {
              project_steps--;
            }
            fluid->set_project_steps(project_steps);
            cout<<"project steps: "<<project_steps<<endl;
          }
          break;
        }
        case SDL_MOUSEMOTION:
        {
          if(event.motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT))
          {
            //cout<<"button 1 pressed and mouse moving"<<endl;
            Float2 pt((float)event.motion.x / WIN_WIDTH, (float)event.motion.y / WIN_HEIGHT);
            Float2 vel(Velocity_scale * (float)event.motion.xrel, Velocity_scale * (float)event.motion.yrel);
            fluid->add_velocity_at_point(pt, vel, 0.03f);
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
  fluid->simulate(t * Time_scale);
  //const float *f = fluid->get_density_array();
  const FluidChannels *fc = fluid->get_channels();

  int w, h;
  fluid_tex->get_dim(w, h);

  Float3 color_a(80.0f, 55.0f, 35.0f);
  Float3 color_b(255.0f, 255.0f, 255.0f);

  GLuint mode = fluid_tex->get_pixel_mode();
  int num_bytes = (mode == GL_RGBA) ? 4 : 3;

  GLubyte *pixels = new GLubyte[w * h * num_bytes];
  for(int i = 0; i < w; i++)
  {
    for(int j = 0; j < h; j++)
    {
      int fluid_idx = i + (w + 2) * j;
      float val = 0.5f * clamp(fc[fluid_idx].data[Fluid_channel_display], -1.0f, 1.0f) + 0.5f;
      Float3 final_color = lerp(color_a, color_b, val);

      for(int oct = 0; oct < 3; oct++)
      {
        pixels[((i * w + j) * num_bytes) + oct] = (GLubyte)final_color[oct];//(val * 255.0f);
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
  float dt = (game_time - Previous_game_time);
  //cout<<dt<<endl;
  Previous_game_time = game_time;

  //fill_dynamic_texture(game_time);
  fill_fluid_texture(dt);

/*
  //HACK
  Float2 pt(0.5f, 0.49f);
  Float2 vel(100.0f * cos(game_time * 0.001f), 100.0f * sin(game_time * 0.001f));
  fluid->add_velocity_at_point(pt, vel, 0.06f);
  fluid->add_density_at_point(pt, Fluid_add_amount, 0.05f);

  Float2 pt2(0.75f, 0.51f);
  Float2 vel2(-100.0f, 0.0f);
  //fluid->add_velocity_at_point(pt2, vel2, 0.1f);

  Float2 pt3(0.25f, 0.5f);
  Float2 vel3(200.0f, 0.0f);
  //fluid->add_velocity_at_point(pt3, vel3, 0.1f);

  Float2 pt4(0.5f + 0.25f * cos(game_time * 0.0005f), 0.5f + 0.25f * sin(game_time * 0.0005f));
  //fluid->add_density_at_point(pt4, Fluid_add_amount, 0.04f);

  Float2 pt5(0.5f + 0.25f * cos(M_PI + game_time * 0.001f), 0.5f + 0.25f * sin(M_PI + game_time * 0.001f));
  fluid->add_density_at_point(pt5, -Fluid_add_amount, 0.05f);
  */

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

  fluid_tex = new Texture(FLUID_DIM, FLUID_DIM, GL_RGB);
  fluid_tex->init();

  fluid = new Fluid2D(FLUID_DIM, FLUID_DIM);

  inflow = new Fluid2DInflow;
  inflow->set_rate(100.0f);
  //fluid->add_interactor(inflow);

  turb = new Fluid2DTurbulenceField;
  turb->set_scale(12.0f);
  turb->set_octaves(2);
  turb->set_speed(0.6f);
  turb->set_strength(1.4f);
  fluid->add_interactor(turb);

  turb_in = new Fluid2DTurbulenceInflow;
  turb_in->set_scale(8.0f);
  turb_in->set_octaves(3);
  turb_in->set_speed(-2.0f);
  turb_in->set_strength(0.2f);
  //turb_in->set_phase(0.0f);
  fluid->add_interactor(turb_in);

  Float2 phase(13.432f, -34.4654f);

  turb_out = new Fluid2DTurbulenceInflow;
  turb_out->set_scale(8.0f);
  turb_out->set_octaves(3);
  turb_out->set_speed(0.5f);
  turb_out->set_strength(-0.2f);
  turb_out->set_phase(phase);
  fluid->add_interactor(turb_out);

  while(true)
  {
    process_events();
    game_loop();
  }

  quit_app();

  delete fluid;
  delete fluid_tex;
  delete inflow;
  delete turb;
  delete turb_in;
  delete turb_out;

	return 0;
}
