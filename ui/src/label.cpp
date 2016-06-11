#include <iostream>
#include "label.h"

using namespace UI;
using namespace std;
using namespace Math;
using namespace Graphics;

Label::Label(Font *f) : RectangularWidget(f)
{
  text = std::string("Label Text");
  rgb = Float3(1.0f, 1.0f, 1.0f);
}

void Label::render()
{
  if(visible)
  {
    glColor3f(1.0f, 1.0f, 1.0f);
    font->print(pos[0], pos[1], text.c_str());
  }
}

void Label::simulate(const double game_time, const double frame_time)
{
  Widget::simulate(game_time, frame_time);
}