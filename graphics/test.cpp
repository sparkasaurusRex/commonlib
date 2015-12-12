#include <time.h>
#include "sdl_game.h"
#include "camera.h"
#include "gpu_hair_sim.h"
#include "perlin.h"

using namespace Graphics;
using namespace PerlinNoise;

enum RenderMode
{
  RENDER_HAIR,
  RENDER_HAIR_TEXTURE,
  RENDER_FORCE_TEXTURE,
  RENDER_UV_TEXTURE,
  NUM_RENDER_MODES
};

class GraphicsApp : public SDLGame
{
public:
  GraphicsApp() : SDLGame(512, 512, "Graphics Test")
  {
    rot_angle = 0.0f;
    color_tex = NULL;
    render_mode = RENDER_HAIR;
    force_tex_dim[0] = 256;
    force_tex_dim[1] = 256;
  }
  ~GraphicsApp()
  {
    delete color_tex;
  }
private:
  void render_hair()
  {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      glLoadIdentity();
      //glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
      glRotatef(rot_angle, 0.0f, 1.0f, 0.0f);
      //glTranslatef(0.0f, -0.3f, 0.0f);
      glScalef(1.2, 1.2, 1.2);

      //cam.render_setup();
      gpu_hair.render();
      //cam.render_cleanup();

    glPopMatrix();
  }

  void render_fullscreen_quad()
  {
    glBegin(GL_QUADS);
      glColor3f(1.0f, 1.0f, 1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, -1.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, -1.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(1.0f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();
  }

  void setup_textured_quad_state()
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  void render_pos_texture()
  {
    setup_textured_quad_state();
    glBindTexture(GL_TEXTURE_2D, gpu_hair.get_pos_tex(0));
    render_fullscreen_quad();
  }

  void render_force_texture()
  {
    setup_textured_quad_state();
    glBindTexture(GL_TEXTURE_2D, gpu_hair.get_force_tex());
    render_fullscreen_quad();
  }

  void render_uv_texture()
  {
    setup_textured_quad_state();
    glBindTexture(GL_TEXTURE_2D, gpu_hair.get_uv_tex());
    render_fullscreen_quad();
  }

  void render_gl()
  {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch(render_mode)
    {
      case RENDER_HAIR_TEXTURE:
        render_pos_texture();
        break;
      case RENDER_FORCE_TEXTURE:
        render_force_texture();
        break;
      case RENDER_UV_TEXTURE:
        render_uv_texture();
        break;
      default:
        render_hair();
        break;
    }
  }

  void update_forces(const float game_time, const float frame_time)
  {
    float speed = 0.0003f;
    float scale = 1.2f;

    //TODO: don't reallocate this every frame!
    int num_bytes = 4;
    GLfloat *pixels = new GLfloat[force_tex_dim[0] *
                                  force_tex_dim[1] *
                                  num_bytes];


    int pix_idx = 0;
    for(int i = 0; i < force_tex_dim[0]; i++)
    {
      for(int j = 0; j < force_tex_dim[1]; j++)
      {
        float v = (float)i / (float)force_tex_dim[0];
        float u = (float)j / (float)force_tex_dim[1];

        float lat = M_PI * u;
        float lon = 0.5f * M_PI * (2.0f * v - 1.0f);

        //TODO: LUT for (lat, lon) -> (x, y, z)
        float r = 1.0f;
        float x =  r * cos(lat) * cos(lon);
        float y =  r * sin(lon);
        float z =  r * sin(lat) * cos(lon);

        Float3 p(x, y, z);
        p.normalize();
        p = p * 1.0f;

        pixels[pix_idx++] = p[0];// + scaled_octave_noise_4d(2, 1.0f, scale, -1.0f, 1.0f, x + game_time * speed, y, z, game_time * speed * 0.2f);
        pixels[pix_idx++] = p[1];// + scaled_octave_noise_4d(2, 1.0f, scale * 0.95, -1.0f, 1.0f, x + 7.15f + game_time * speed, y + 13.76f, z + 12.74f, game_time * speed * 0.2f);
        pixels[pix_idx++] = p[2];// + scaled_octave_noise_4d(2, 1.0f, scale * 1.2f, -1.0f, 1.0f, x + 3.12f + game_time * speed, y + 67.12f, z - 4.1784f, game_time * speed * 0.2f);

        //hair height multiplier
        float hair_height = 1.0f;
        float h = scaled_octave_noise_3d(3, 1.0f, scale * 1.1f, 0.0, hair_height, x + 7.12f, y + 4.12f, game_time * speed);
        h = h * h;
        pixels[pix_idx++] = h;
      }
    }

    gpu_hair.update_forces(pixels);

    delete pixels;
  }

  void game_loop(const float game_time, const float frame_time)
  {
    update_forces(game_time, frame_time);
    gpu_hair.simulate(game_time, frame_time);
    if(!paused)
    {
      rot_angle += 10.0f * frame_time;
    }
  }

  void user_init()
  {
    gpu_hair.set_num_hairs(10000);
    gpu_hair.set_num_segments(6);
    gpu_hair.set_force_tex_dim(force_tex_dim[0], force_tex_dim[1]);
    gpu_hair.init();

    color_tex = new Texture("data/grass.tif");
    color_tex->load();
    gpu_hair.set_color_tex(color_tex);

    Float3 cam_pos(0.0f, 0.5f, -10.0f);
    cam.set_pos(cam_pos);
    cam.set_lookat(Float3(0.0f, 0.0f, 0.0f) - cam_pos);
    cam.set_up(Float3(0.0f, 1.0f, 0.0f));
  }
  void user_run() {}
  void user_process_event(const SDL_Event &event)
  {
    switch(event.type)
    {
      case SDL_KEYUP:
        switch(event.key.keysym.sym)
        {
          case ' ':
            paused = !paused;
            break;
          case '1':
            render_mode = RENDER_HAIR;
            break;
          case '2':
            render_mode = RENDER_HAIR_TEXTURE;
            break;
          case '3':
            render_mode = RENDER_FORCE_TEXTURE;
            break;
          case '4':
            render_mode = RENDER_UV_TEXTURE;
            break;
        }
        break;
    }
  }

  GPUHairSim gpu_hair;
  RenderMode render_mode;
  Camera cam;
  float rot_angle;

  bool paused;

  Texture *color_tex;

  unsigned int force_tex_dim[2];
};

int main(int argc, char **argv)
{
  srand(time(NULL));

  GraphicsApp app;
  app.init();
  app.run();
  return 0;
}

/*
#include <GL/glew.h>

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
#include "render_surface.h"
#include "render_surface_combiner.h"

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

RenderSurface rsa, rsb;
RenderSurfaceCombiner rsc;

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
  glewInit();

  std::string vs_name("data/shaders/passthrough.vs");
  std::string fs_name("data/shaders/downsample.fs");

  rsa.set_fbo_res(WIN_WIDTH, WIN_HEIGHT);
  rsa.set_shader_names(vs_name, fs_name);
  rsa.init();

  rsb.set_fbo_res(WIN_WIDTH, WIN_HEIGHT);
  rsb.set_shader_names(vs_name, fs_name);
  rsb.init();

  rsc.set_shader_names(vs_name, std::string("data/shaders/fb_additive_combine.fs"));
  rsc.set_surfaces(&rsa, &rsb, &rsa, &rsb);
  rsc.init();
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
  return;
  fluid->simulate(t);
  const float *f;// = fluid->get_density_array();

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

  rsa.capture();

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

  rsa.release();

  //rsb.capture();
  //rsa.render();
  //rsb.release();

  //rsc.render();
  rsa.render();

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
*/
