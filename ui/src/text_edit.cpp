#include "text_edit.h"

using namespace UI;

TextEdit::TextEdit(Font *f) : RectangularWidget(f)
{
  dim[0] = 100.0f;
  dim[1] = 25.0f;

  text = "TextEdit";
}

void TextEdit::process_event(const SDL_Event &e)
{

}

void TextEdit::init()
{
}

void TextEdit::simulate(const float dt)
{

}

void TextEdit::render()
{
  //first, draw a rectangle background
  glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex3f(pos[0], pos[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1], 0.0f);
    glVertex3f(pos[0], pos[1] - dim[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1] - dim[1], 0.0f);
  glEnd();

  //render the text
  float font_height = font->get_height();
  float border_y = (dim[1] - font_height) / 2.0f;
  float font_y = pos[1] - border_y;
  glColor3f(1.0f, 1.0f, 1.0f);
  font->print(pos[0] + 5.0f, font_y, text.c_str());
}
