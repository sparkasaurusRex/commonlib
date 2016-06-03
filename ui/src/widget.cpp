#include "widget.h"

using namespace UI;
using namespace Graphics;

Widget::Widget(Font *f)
{
  font = f;
  visible = false;
  has_focus = false;
  has_tooltip = false;
  hover_timer = -1.0f;
  hovering = false;
  enabled = true;
}

void Widget::render_prep()
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Widget::render_cleanup()
{

}

void Widget::simulate(const double game_time, const double frame_time)
{
  //see if we're hovering
  /*int mouse_x, mouse_y;
  Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);
  if (hit_test(mouse_x, mouse_y))
  {
    hovering = true;
  }
  hovering = false;
  */
}