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

#define SDL_GAME_DEFAULT_WIDTH    640
#define SDL_GAME_DEFAULT_HEIGHT   480

class SDLGame
{
public:
  //SDLGame();
  SDLGame(const int w = SDL_GAME_DEFAULT_WIDTH,
          const int h = SDL_GAME_DEFAULT_HEIGHT,
          const std::string title = "Game");

  ~SDLGame();

  void init();
  void run();
  void process_events();

protected:
  virtual void game_loop() = 0;

  virtual void user_init() = 0;
  virtual void user_run() = 0;
  virtual void user_process_event(const SDL_Event &event) = 0;

  virtual void render_gl() = 0;

  void init_sdl();
  void quit_app();

  int resolution[2];
  SDL_Window *win;
  SDL_GLContext gl_context;
  std::string window_title;

  double last_game_time;

  DebugConsole console;
};

#endif //__SDL_GAME_H__
