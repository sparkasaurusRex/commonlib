#include "sdl_game.h"
#include "widget_wrangler.h"
#include "label.h"
#include "push_button.h"
#include "check_button.h"
#include "toolbox.h"
#include "radial_push_button.h"
#include "menu.h"
#include "curve_editor.h"
#include "text_edit.h"
#include "meter.h"

#define TOOLBOX_SIZE 4
#define NUM_RPB 12

using namespace UI;
using namespace std;
using namespace Math;

void pb_callback(const SDL_Event &e);

class App : public SDLGame
{
public:
  App() : SDLGame(512, 512, "UI Test")
  {
    widget_font = NULL;
    pb_tex[0] = new Texture("../graphics/data/test.tga");
    pb_tex[1] = new Texture("../graphics/data/smoke_alpha_01.tga");
  }
  ~App()
  {
    delete pb_tex[0];
    delete pb_tex[1];
  }

  void randomize_bg_color()
  {
    //bg_color = Float3(random(0.0f, 1.0f), random(0.0f, 1.0f), random(0.0f, 1.0f));
    FILE *f = fopen("test.curve", "wb");
    curve.fwrite(f);
    fclose(f);
  }

  void randomize_tb_pos()
  {
    Float2 new_pos(random(40.0f, 300.0f), random(20.0f, 500.0f));
    tb.translate(new_pos);
  }

private:
  void render_gl()
  {
    glClearColor(bg_color[0], bg_color[1], bg_color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ww.render();
  }
  void game_loop(const float game_time, const float frame_time)
  {
    ww.simulate(frame_time);

    meter.set_percent(0.5f * cos(game_time * 0.001f) + 0.5f);
  }

  void user_init()
  {
    pb_tex[0]->load();
    pb_tex[1]->load();

    label.set_font(widget_font);
    label.set_text(std::string("label text 1"));
    label.translate(Float2(10.0f, 400.0f));
    label.init();
    //ww.add_widget(&label);

    te.set_font(widget_font);
    te.translate(Float2(300.0f, 300.0f));
    te.init();
    //ww.add_widget(&te);

    for(int i = 0; i < TOOLBOX_SIZE; i++)
    {
      pb[i].set_font(widget_font);
      pb[i].set_text(std::string(""));
      pb[i].set_click_callback(pb_callback);
      pb[i].set_texture(0, pb_tex[0]);
      pb[i].set_texture(1, pb_tex[1]);
      pb[i].init();
      tb.add_button(&pb[i]);
      //ww.add_widget(&pb[i]);
    }

    tb.make_vertical();
    tb.translate(Float2(10.0f, 100.0f));
    tb.set_font(widget_font);
    tb.set_button_dim(Float2(64.0f, 64.0f));
    tb.init();
    //ww.add_widget(&tb);

    for(int i = 0; i < NUM_RPB; i++)
    {
      rpb[i].set_radii(Float2(30.0f, 90.0f));

      float arc_offset = 0.0f;
      float arc_width = 2.0f * M_PI / (float)NUM_RPB;
      rpb[i].set_arc(Float2(i * arc_width + 0.05f + arc_offset, (i + 1) * arc_width - 0.05f + arc_offset));
      rpb[i].set_center(Float2(180.0f, 380.0f));
      rpb[i].set_click_callback(pb_callback);
      rpb[i].init();
      //ww.add_widget(&rpb[i]);
    }

    ce.set_curve(&curve);
    ce.set_font(widget_font);
    ce.translate(Float2(100.0f, 30.0f));
    ce.scale(Float2(400.0f, 240.0f));
    ce.init();
    ww.add_widget(&ce);

    meter.set_font(widget_font);
    meter.translate(Float2(100.0f, 350.0f));
    meter.scale(Float2(100.0f, 20.0f));
    meter.init();
    ww.add_widget(&meter);
  }

  void user_run()
  {
    //label.show();
    //pb.show();
    tb.show();
    ce.show();
    te.show();
    meter.show();
    //menu.show();

    for(int i = 0; i < NUM_RPB; i++)
    {
      rpb[i].show();
    }
  }

  void user_process_event(const SDL_Event &e)
  {
    ww.process_event(e);
  }

  Float3 bg_color;
  WidgetWrangler ww;
  Label label;
  CheckButton pb[TOOLBOX_SIZE];
  ToolBox tb;
  TextEdit te;

  CurveEditor ce;
  Curve curve;
  //Menu menu;
  Meter meter;


  RadialPushButton rpb[NUM_RPB];

  Texture *pb_tex[2];
};

App *my_app = NULL;

void pb_callback(const SDL_Event &e)
{
  my_app->randomize_bg_color();
  //my_app->randomize_tb_pos();
  cout<<"pb_callback!"<<endl;
}

int main(int argc, char **argv)
{
  my_app = new App;
  my_app->init();
  my_app->run();
  delete my_app;

  return 0;
}
