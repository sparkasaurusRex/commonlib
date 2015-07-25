#include "sdl_game.h"

class MySDLGame : public SDLGame
{
public:
  MySDLGame() {}
  ~MySDLGame() {}

  void init() { init_sdl(); }
  void run() { while(true) { process_events(); game_loop(); } }
private:
  void game_loop() {}
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
};

int main(int argc, char **argv)
{
  MySDLGame game;
  game.init();
  game.run();
  return 0;
}
