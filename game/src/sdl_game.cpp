#include <assert.h>
#include <iostream>
#include "sdl_game.h"

using namespace std;

SDLGame::SDLGame(const int w, const int h, std::string title)
{
  resolution[0] = w;
  resolution[1] = h;

  win = NULL;
  gl_context = NULL;

  window_title = title;

  last_game_time = 0.0f;
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
  console.init();
}

void SDLGame::run()
{
  while(true)
  {
    Uint32 ticks = SDL_GetTicks();
    double game_time = (double)ticks;
    double frame_time = (game_time - last_game_time) / 1000.0f;
    last_game_time = game_time;

    user_run();
    process_events();
    game_loop();

    render_gl();

    console.simulate(frame_time);
    console.render_gl();

    glFlush();
    SDL_GL_SwapWindow(win);

    //if(console.is_active()) cout<<"true"<<endl; else cout<<"false"<<endl;
  }
}

void SDLGame::process_events()
{
  //base level of event processing every app should have
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);
  assert(keystate);
  if(keystate[SDL_SCANCODE_ESCAPE]) quit_app();

  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    if(event.type == SDL_KEYUP)
    {
      switch(event.key.keysym.sym)
      {
      case '`':
        console.activate(!console.is_active());
        break;

      //ugh... I hate how hacky this is
      //TODO: non-private traverse_command_history()
      case SDLK_UP:
      case SDLK_DOWN:
        console.receive_char(event.key.keysym.sym);
        break;
      case SDLK_RETURN:
      //case SDLK_ENTER:
        if(console.is_active())
        {
          console.execute();
        }
        break;
      default:
        break;
      }
    }

    if(event.type == SDL_TEXTINPUT)
    {
      //cout<<event.text.text<<endl;
      if(console.is_active())
      {
        if(event.text.text[0] != '`')
        {
          console.receive_char(event.text.text[0]);
        }
      }
    }

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

    //SDL_EnableUNICODE(SDL_ENABLE);
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
