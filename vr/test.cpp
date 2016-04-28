#if defined (_WIN32)
#include <Windows.h>
#include <GL/glew.h>
#endif //_WIN32

#include <iostream>

#include "sdl_game.h"
#include "vr.h"

using namespace std;
using namespace VR;

VRContext vr_context;

class VRGame : public SDLGame
{
private:

  void game_loop(const double game_time, const double frame_time) {}
  void user_init() {}
  void user_run() {}
  void user_process_event(const SDL_Event &event) {}
  void render_gl()
  {
    //render once for each eye
    vr_context.retrieve_eye_poses();
    for (int eye = 0; eye < 2; eye++)
    {
      vr_context.render_capture(eye);
      glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      vr_context.render_release(eye);
    }
    vr_context.finalize_render();
  }
public:
  VRGame() : SDLGame(640, 480, "VR Test") {}
  ~VRGame() {}
};

int main(int argc, char **argv)
{
  vr_context.init();

  VRGame game;
  game.enable_vsync(false);
  game.init();

  vr_context.bind(&game);
  
  game.run();

  vr_context.deinit();

  return 0;
}