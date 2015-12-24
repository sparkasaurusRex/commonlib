#include "audio.h"
#include "sdl_game.h"


//using namespace Game;
using namespace Audio;

class TestApp : public SDLGame
{
public:
  TestApp() : SDLGame(512, 512, "Audio Test") {}
  ~TestApp() {}

  void user_init()
  {
      audio_imp.init();
  }

  void user_run() {}

  void user_process_event(const SDL_Event &event) {}

  void game_loop(const float game_time, const float frame_time)
  {
    audio_imp.simulate(game_time, frame_time);
  }

  void render_gl()
  {

  }
private:
  AudioImp audio_imp;
};

int main(int argc, char **argv)
{
  TestApp app;
  app.init();
  app.run();

  return 0;
}
