#ifndef __SDL_GAME_H__
#define __SDL_GAME_H__

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <SDL2/SDL.h>

#define SDL_GAME_DEFAULT_WIDTH    640
#define SDL_GAME_DEFAULT_HEIGHT   480

class SDLGame
{
public:
  //SDLGame();
  SDLGame(const int w = SDL_GAME_DEFAULT_WIDTH, const int h = SDL_GAME_DEFAULT_HEIGHT);
  ~SDLGame();

  virtual void init() = 0;
  virtual void run() = 0;

protected:
  virtual void game_loop() = 0;
  virtual void process_events() = 0;

  void init_sdl();
  void quit_app();

  int resolution[2];
  SDL_Window *win;
  SDL_GLContext gl_context;
};

#endif //__SDL_GAME_H__
