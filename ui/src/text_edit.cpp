#include <iostream>
#include "text_edit.h"

using namespace UI;
using namespace std;
using namespace Math;
using namespace Graphics;

TextEdit::TextEdit(Font *f) : RectangularWidget(WIDGET_TEXT_EDIT, f)
{
  dim[0] = 100.0f;
  dim[1] = 25.0f;

  text = "TextEdit";
  selection[0] = -1;
  selection[1] = -1;
}

void TextEdit::process_event(const SDL_Event &e, const Float2 offset)
{
  if(e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
  {
    selection[0] = 0;
    selection[1] = (int)text.size();
    SDL_StartTextInput();
  }

  //cout<<"TextEdit::process_event()"<<endl;
  switch(e.type)
  {
    case SDL_KEYUP:
    {
      switch(e.key.keysym.sym)
      {
        case SDLK_RETURN:
          //relinquish focus
          has_focus = false;
            //cout<<"TextEdit::process_event(): return pressed!"<<endl;
          break;
      }
      break;
    }
    case SDL_TEXTINPUT:
      if(selection[0] != -1 && selection[1] != -1)
      {
        text.erase(selection[0], selection[1] - selection[0]);
      }
      text += e.text.text[0];

      //deselect
      selection[0] = selection[1] = -1;
      break;
    case SDL_TEXTEDITING:
      cout<<e.edit.text<<endl;
      break;
  }
  if(e.type == SDL_TEXTINPUT)
  {

  }
}

void TextEdit::init()
{
}

void TextEdit::simulate(const double game_time, const double frame_time)
{
  RectangularWidget::simulate(game_time, frame_time);
}

void TextEdit::render()
{
  //first, draw a rectangle background
  glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.7f, 0.7f, 0.85f);
    glVertex3f(pos[0], pos[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1], 0.0f);
    glVertex3f(pos[0], pos[1] - dim[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1] - dim[1], 0.0f);
  glEnd();

  //render the text
  float font_height = font->get_height();
  float border_y = (dim[1] - font_height) / 2.0f;
  float font_y = pos[1] - border_y;
  glColor3f(0.0f, 0.0f, 0.0f);
  font->print(pos[0] + 5.0f, font_y, text.c_str());
}
