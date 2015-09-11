#include <iostream>
#include "push_button.h"

using namespace std;
using namespace UI;

void PushButton::process_event(const SDL_Event &event)
{
  if(event.type == SDL_MOUSEBUTTONDOWN)
  {
    int mouse_x, mouse_y;
    Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    if(hit_test(mouse_x, mouse_y))
    {
      click_capture = true;
    }
  }
  if(event.type == SDL_MOUSEBUTTONUP)
  {
    int mouse_x, mouse_y;
    Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);

    if(click_capture && hit_test(mouse_x, mouse_y))
    {
      click_capture = false;
      cout<<"button clicked!"<<endl;
    }
  }
}

void PushButton::render()
{
  Label::render();

  //cout<<"dim: "<<dim<<endl;

  glLineWidth(2.0f);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glBegin(GL_LINES);
    glVertex3f(pos[0], pos[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1], 0.0f);

    glVertex3f(pos[0], pos[1] - dim[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1] - dim[1], 0.0f);

    glVertex3f(pos[0], pos[1], 0.0f);
    glVertex3f(pos[0], pos[1] - dim[1], 0.0f);

    glVertex3f(pos[0] + dim[0], pos[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1] - dim[1], 0.0f);
  glEnd();
}
