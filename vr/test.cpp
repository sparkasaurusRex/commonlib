#if defined (_WIN32)
#include <Windows.h>
#include <GL/glew.h>
#endif //_WIN32

#include <iostream>

#include "sdl_game.h"
#include "vr.h"

using namespace std;
using namespace VR;

class VRGame : public SDLGame
{
private:
  void game_loop(const double game_time, const double frame_time) {}
  void user_init() {}
  void user_run() {}
  void user_process_event(const SDL_Event &event) {}
  void render_gl()
  {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
public:
  VRGame() : SDLGame(640, 480, "VR Test") {}
  ~VRGame() {}
};

int main(int argc, char **argv)
{
  VRContext vr_context;
  vr_context.init();

  VRGame game;
  game.init();
  
  game.run();

  vr_context.deinit();

  return 0;
}