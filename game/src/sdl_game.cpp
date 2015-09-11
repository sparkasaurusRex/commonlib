#include <assert.h>
#include <iostream>
#include <GL/glew.h>

#include "sdl_game.h"
#include "label.h"

using namespace std;

SDLGame::SDLGame(const int w, const int h,
                 std::string title,
                 const int gl_major_version, const int gl_minor_version)
{
  resolution[0] = w;
  resolution[1] = h;

  gl_version[0] = gl_major_version;
  gl_version[1] = gl_minor_version;

  win = NULL;
  gl_context = NULL;

  window_title = title;

  last_game_time = 0.0f;

  recording_movie = false;
  movie_frame_counter = 0;

  font = NULL;
#if defined(__APPLE__)
  font_face = "/Library/Fonts/Andale Mono.ttf";
#endif //__APPLE__
  font_height = 24;
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
  font = new Font(font_face.c_str(), font_height);
  font->init();
  title_screen.set_font(font);

  widget_font = new Font(font_face.c_str(), 12);
  widget_font->init();
  
  fps_label.set_font(widget_font);
  fps_label.set_text(std::string("fps"));
  fps_label.init();
  fps_label.translate(Float2(10.0f, 10.0f));
  fps_label.show();

  user_init();
  console.init();
}

void SDLGame::run()
{
  title_screen.play();
  title_screen.set_text(window_title);

  Uint32 ticks = SDL_GetTicks();
  last_game_time = (double)ticks;
  while(true)
  {
    ticks = SDL_GetTicks();
    double game_time = (double)ticks;
    double frame_time = (game_time - last_game_time) / 1000.0f;
    last_game_time = game_time;

    float fps = 1.0f / frame_time;

    user_run();
    process_events();
    game_loop(game_time, frame_time);

    std::string fps_text = std::string("fps: ") + std::to_string(fps);
    fps_label.set_text(fps_text);
    fps_label.simulate(frame_time);

    if(title_screen.is_active())
    {
      title_screen.simulate(frame_time);
      title_screen.render_gl();
    }
    else
    {
      render_gl();
      fps_label.render();
    }

    console.simulate(frame_time);
    console.render_gl();

    if(recording_movie)
    {
      screenshot();
    }

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
      case SDLK_UP:
        console.traverse_command_history(1);
        break;
      case SDLK_DOWN:
        console.traverse_command_history(-1);
        break;
      case SDLK_RETURN:
      //case SDLK_ENTER:
        if(console.is_active()) { console.execute(); }
        break;
      case SDLK_BACKSPACE:
        if(console.is_active()) { console.backspace(); }
        break;
      case SDLK_F12:
        if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT])
        {
          recording_movie = !recording_movie;
          if(recording_movie) { begin_video_capture(); }
          else { end_video_capture(); }
        }
        else { screenshot(); }
        break;
      default:
        break;
      }
    }

    if(event.type == SDL_TEXTINPUT)
    {
      if(console.is_active())
      {
        if(event.text.text[0] != '`')
        {
          console.receive_char(event.text.text[0]);
        }
      }
    }

    if(event.type == SDL_QUIT) { quit_app(); }
    if(!console.is_active()) { user_process_event(event); }
  }
}

void SDLGame::screenshot()
{
  cout<<"taking screenshot..."<<endl;
  SDL_Surface *image = SDL_CreateRGBSurface(SDL_SWSURFACE, resolution[0], resolution[1], 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

  glReadPixels(0, 0, resolution[0], resolution[1], GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

  char filename[256];
  sprintf(filename, "capture/%s%i.bmp", window_title.c_str(), movie_frame_counter++);
  cout<<"writing "<<filename<<endl;
  SDL_SaveBMP(image, filename);
  SDL_FreeSurface(image);
}

void SDLGame::init_sdl()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    //TODO: should probably query to hardware first to determine support
    //or - maybe just offer an option of core vs. compatibility contexts
    if(gl_version[0] > 0 && gl_version[1] > 0)
    {
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_version[0]);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_version[1]);
    }
    else
    {
      // seems like it always wants 4.1 (or probably the largest supported version
      // when I call this function)
      // NOTE: no immediate mode functionality will work with PROFILE_CORE
      //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    }

    //Create window
    win = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution[0], resolution[1], SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    assert(win);

    gl_context = SDL_GL_CreateContext(win);
    assert(gl_context);

    SDL_GL_SetSwapInterval(1);

    glewInit();

    cout<<"Initializing OpenGL..."<<endl;
    cout<<"version "<<glGetString(GL_VERSION)<<endl;//major_version<<"."<<minor_version<<endl;
    //cout<<"extensions: "<<endl<<glGetString(GL_EXTENSIONS)<<endl;
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
