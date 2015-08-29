#include "sdl_game.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

class MySDLGame : public SDLGame
{
public:
  MySDLGame() {}
  ~MySDLGame() {}

private:
  void game_loop(const float game_time, const float frame_time) {}
  void user_init() {}
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
