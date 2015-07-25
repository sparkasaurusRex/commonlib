#include <assert.h>
#include "sdl_game.h"

SDLGame::SDLGame(const int w, const int h, std::string title)
{
  resolution[0] = w;
  resolution[1] = h;

  win = NULL;
  gl_context = NULL;

  window_title = title;
}

SDLGame::~SDLGame()
{
  if(gl_context)
  {
    SDL_GL_DeleteContext(gl_context);
    gl_context = NULL;
  }
  if(win)
  {
    SDL_DestroyWindow(win);
    win = NULL;
  }
}

void SDLGame::init()
{
  init_sdl();
  user_init();
}

void SDLGame::run()
{
  while(true)
  {
    user_run();
    process_events();
    game_loop();
  }
}

void SDLGame::process_events()
{
  //base level of event processing every app should have
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);
  if(keystate[SDLK_ESCAPE]) quit_app();

  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    if(event.type == SDL_QUIT)
    {
      quit_app();
    }
    user_process_event(event);
  }
}

void SDLGame::init_sdl()
{
	if(SDL_Init( SDL_INIT_VIDEO ) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    //Create window
    win = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution[0], resolution[1], SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    assert(win);

    gl_context = SDL_GL_CreateContext(win);
    assert(gl_context);

    SDL_GL_SetSwapInterval(1);
  }
}

void SDLGame::quit_app()
{
    SDL_GL_DeleteContext(gl_context);
    gl_context = NULL;

    SDL_DestroyWindow(win);
    win = NULL;

    SDL_Quit();
    exit(0);
}
