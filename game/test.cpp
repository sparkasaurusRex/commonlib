#include "sdl_game.h"

class MySDLGame : public SDLGame
{
public:
  MySDLGame() {}
  ~MySDLGame() {}

  //void init();
  //void run();
  //void process_events();
private:
  void game_loop() {}
  void user_init() {}
  void user_run() {}
  void user_process_event(const SDL_Event &event) {}
};

int main(int argc, char **argv)
{
  MySDLGame game;
  game.init();
  game.run();
  return 0;
}
