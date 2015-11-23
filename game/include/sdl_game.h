#ifndef __SDL_GAME_H__
#define __SDL_GAME_H__

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <SDL2/SDL.h>

#include <string>

#include "console.h"
#include "cinematics/fade_screen.h"
#include "label.h"

//TODO: framerate counter

#define SDL_GAME_DEFAULT_WIDTH    640
#define SDL_GAME_DEFAULT_HEIGHT   480

class SDLGame
{
public:
  //SDLGame();
  SDLGame(const int w = SDL_GAME_DEFAULT_WIDTH,
          const int h = SDL_GAME_DEFAULT_HEIGHT,
          const std::string title = "Game",
          const int _gl_context_profile = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
          const int gl_major_version = -1,
          const int gl_minor_version = -1);

  ~SDLGame();

  void set_gl_version(const int major, const int minor) { gl_version[0] = major; gl_version[1] = minor; }

  void init();
  void run();
  void process_events();

  void begin_video_capture() { recording_movie = true; movie_frame_counter = 0; }
  void end_video_capture() { recording_movie = false; movie_frame_counter = 0; }

protected:
  virtual void game_loop(const float game_time, const float frame_time) = 0;

  virtual void user_init() = 0;
  virtual void user_run() = 0;
  virtual void user_process_event(const SDL_Event &event) = 0;

  virtual void render_gl() = 0;

  void init_sdl();
  void quit_app();

  void screenshot();

  bool recording_movie;
  int movie_frame_counter;

  int resolution[2];
  SDL_Window *win;
  SDL_GLContext gl_context;
  int gl_version[2];
  int gl_context_profile;
  std::string window_title;

  double last_game_time;

  std::string font_face;
  unsigned int font_height;

  Font *font;    //font we want to use for mostly everything
  DebugConsole console;

  Font *widget_font;
  UI::Label fps_label;

  FadeScreen title_screen;
};

#endif //__SDL_GAME_H__
