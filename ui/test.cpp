#include "sdl_game.h"
#include "label.h"

using namespace UI;

class App : public SDLGame
{
public:
  App() : SDLGame(512, 512, "UI Test") { widget_font = NULL; }
  ~App() {}
private:
  void render_gl()
  {
    glColor3f(1.0f, 1.0f, 1.0f);
    label.render();
  }
  void game_loop(const float game_time, const float frame_time)
  {
    label.simulate(frame_time);
  }

  void user_init()
  {
    widget_font = new Font(font_face.c_str(), 12);
    widget_font->init();
    label.set_font(widget_font);
    label.show();
    label.set_text(std::string("label text 1"));
    label.translate(Float2(10.0f, 490));
  }
  void user_run() {}
  void user_process_event(const SDL_Event &e) {}

  Font *widget_font;

  Label label;
};

int main(int argc, char **argv)
{
  App my_app;
  my_app.init();
  my_app.run();

  return 0;
}
