#include "sdl_game.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

using namespace Math;

class MySDLGame : public SDLGame
{
public:
  MySDLGame() : SDLGame(640, 480, "Game Test") {}
  ~MySDLGame() {}

private:
  void game_loop(const float game_time, const float frame_time) {}
  void user_init()
  {
    float float_vars[5];
    bool bool_vars[3];
    Float3 float3_vars[2];

    console.register_variable(&float_vars[0], "apple_sauce");
    console.register_variable(&float_vars[1], "peanut_butter");
    console.register_variable(&float_vars[2], "app");
    console.register_variable(&float_vars[3], "flossy");
    console.register_variable(&float_vars[4], "razzle");

    console.register_variable(&bool_vars[0], "razzle2");
    console.register_variable(&bool_vars[1], "RAZZLE");
    console.register_variable(&bool_vars[2], "pig_snack");

    console.register_variable(&float3_vars[0], "mouse");
    console.register_variable(&float3_vars[1], "sauce");
    console.register_variable(&float3_vars[2], "flossy");
  }
  void user_run() {}
  void user_process_event(const SDL_Event &event) {}
  void render_gl()
  {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  }
};

int main(int argc, char **argv)
{
  MySDLGame game;
  game.init();
  game.run();
  return 0;
}
