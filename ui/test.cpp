#include "sdl_game.h"
#include "widget_wrangler.h"
#include "label.h"
#include "push_button.h"
#include "check_button.h"
#include "toolbox.h"
#include "radial_push_button.h"
#include "menu.h"
#include "curve_editor.h"

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
    bg_color = Float3(random(0.0f, 1.0f), random(0.0f, 1.0f), random(0.0f, 1.0f));
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
    label.simulate(frame_time);
    //pb.simulate(frame_time);
    tb.simulate(frame_time);
    ce.simulate(frame_time);
    //menu.simulate(frame_time);
  }

  void user_init()
  {
    pb_tex[0]->load();
    pb_tex[1]->load();

    label.set_font(widget_font);
    label.set_text(std::string("label text 1"));
    label.translate(Float2(10.0f, 490.0f));
    label.init();
    ww.add_widget(&label);

    for(int i = 0; i < TOOLBOX_SIZE; i++)
    {
      pb[i].set_font(widget_font);
      pb[i].set_text(std::string(""));
      //pb[i].scale(Float2(60.0f, 60.0f));
      //pb[i].translate(Float2(10.0f, 450.0f));
      pb[i].set_click_callback(pb_callback);
      pb[i].set_texture(0, pb_tex[0]);
      pb[i].set_texture(1, pb_tex[1]);
      pb[i].init();
      tb.add_button(&pb[i]);
    //ww.add_widget(&pb);
    }

    tb.make_vertical();
    tb.translate(Float2(10.0f, 100.0f));
    tb.set_font(widget_font);
    tb.set_button_dim(Float2(64.0f, 64.0f));
    tb.init();
    ww.add_widget(&tb);

    for(int i = 0; i < NUM_RPB; i++)
    {
      rpb[i].set_radii(Float2(30.0f, 90.0f));

      float arc_offset = 0.0f;
      float arc_width = 2.0f * M_PI / (float)NUM_RPB;
      rpb[i].set_arc(Float2(i * arc_width + 0.05f + arc_offset, (i + 1) * arc_width - 0.05f + arc_offset));
      rpb[i].set_center(Float2(180.0f, 380.0f));
      rpb[i].set_click_callback(pb_callback);
      rpb[i].init();
      ww.add_widget(&rpb[i]);
    }

    ce.set_curve(&curve);
    ce.init();
    ce.translate(Float2(100.0f, 30.0f));
    ce.scale(Float2(400.0f, 240.0f));
    ww.add_widget(&ce);
  }

  void user_run()
  {
    //label.show();
    //pb.show();
    tb.show();
    ce.show();
    //menu.show();

    for(int i = 0; i < NUM_RPB; i++)
    {
      rpb[i].show();
    }
  }

  void user_process_event(const SDL_Event &e)
  {
    //pb.process_event(e);
    tb.process_event(e);
    ce.process_event(e);
    //menu.process_event(e);

    for(int i = 0; i < NUM_RPB; i++)
    {
      rpb[i].process_event(e);
    }
  }

  Float3 bg_color;
  WidgetWrangler ww;
  Label label;
  CheckButton pb[TOOLBOX_SIZE];
  ToolBox tb;

  CurveEditor ce;
  Curve curve;
  //Menu menu;

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
