#include "sdl_game.h"
#include "widget_wrangler.h"
#include "label.h"
#include "push_button.h"

using namespace UI;

class App : public SDLGame
{
public:
  App() : SDLGame(512, 512, "UI Test") { widget_font = NULL; }
  ~App() {}
private:
  void render_gl()
  {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ww.render();
  }
  void game_loop(const float game_time, const float frame_time)
  {
    label.simulate(frame_time);
    pb.simulate(frame_time);
  }

  void user_init()
  {
    label.set_font(widget_font);
    label.set_text(std::string("label text 1"));
    label.translate(Float2(10.0f, 490.0f));
    label.init();
    ww.add_widget(&label);

    pb.set_font(widget_font);
    pb.set_text(std::string("Button"));
    pb.scale(Float2(100.0f, 20.0f));
    pb.translate(Float2(10.0f, 450.0f));
    pb.init();
    ww.add_widget(&pb);
  }

  void user_run()
  {
    label.show();
    pb.show();
  }

  void user_process_event(const SDL_Event &e)
  {
    pb.process_event(e);
  }

  WidgetWrangler ww;
  Label label;
  PushButton pb;
};

int main(int argc, char **argv)
{
  App my_app;
  my_app.init();
  my_app.run();

  return 0;
}
