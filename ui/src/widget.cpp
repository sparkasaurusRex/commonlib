#include "widget.h"

using namespace UI;

void Widget::render_prep()
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Widget::render_cleanup()
{

}