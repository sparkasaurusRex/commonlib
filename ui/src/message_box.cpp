#include "message_box.h"

using namespace UI;
using namespace Graphics;

Message::Message(Font *f) : RectangularWidget(f)
{
  main_text = "Useful message";
  //buttons = NULL;
  timer.set(5.0);
}

void Message::show(const bool d)
{
  visible = d;
  if (visible)
  {
    timer.start();
  }
}

void Message::simulate(const double game_time, const double frame_time)
{
  Widget::simulate(game_time, frame_time);

  if (visible && timer.elapsed())
  {
    visible = false;
  }
}

void Message::render()
{
  if (visible)
  {
    font->print(pos[0], pos[1], main_text.c_str());
  }
}