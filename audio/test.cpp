#include "audio.h"
#include "sdl_game.h"


//using namespace Game;
using namespace Audio;

class TestApp : public SDLGame
{
public:
  TestApp() : SDLGame(512, 512, "Audio Test")
  {
    bg_loop = -1;
  }

  ~TestApp() {}

  void user_init()
  {
    audio_imp.init();
    //bg_loop = audio_imp.load_music("data/183881__erokia__elementary-wave-11.wav");
    bg_loop = audio_imp.load_music("data/130982__reacthor__drone-of-suspense.wav");
    //bg_loop = audio_imp.load_music("data/underwater_ambient.flac");
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
            audio_imp.play_music(bg_loop, -1);
            break;
        }
        break;
    }
  }

  void game_loop(const float game_time, const float frame_time)
  {
    audio_imp.simulate(game_time, frame_time);
  }

  void render_gl()
  {

  }
private:
  AudioImp audio_imp;
  unsigned int bg_loop;
};

int main(int argc, char **argv)
{
  TestApp app;
  app.init();
  app.run();

  return 0;
}
