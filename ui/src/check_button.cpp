#include <iostream>
#include "check_button.h"

using namespace std;
using namespace UI;

void CheckButton::process_event(const SDL_Event &event)
{
  //PushButton::process_event(event);
  //cout<<"checkbutton::process_event"<<endl;

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
      checked = !checked;
      cout<<(int)checked<<endl;
      if(click_callback)
      {
        click_callback(event);
      }
      //cout<<"button clicked!"<<endl;
    }
    click_capture = false;
  }
}

void CheckButton::render()
{
  //cout<<"checkbutton::render()"<<endl;
  //if(checked) { cout<<"checked"<<endl; }
  glLineWidth(1.0f);
  if(click_capture)
  {
    glColor4f(click_rgb[0], click_rgb[1], click_rgb[2], 1.0f);
  }
  else
  {
    glColor4f(rgb[0], rgb[1], rgb[2], 1.0f);
  }

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

  if(textures[0] && !checked)
  {
    textures[0]->render_gl();
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBegin(GL_TRIANGLE_STRIP);
      glVertex3f(pos[0], pos[1], 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(pos[0] + dim[0], pos[1], 0.0f);
      glTexCoord2f(0.0f, 0.0f);

      glVertex3f(pos[0], pos[1] - dim[1], 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(pos[0] + dim[0], pos[1] - dim[1], 0.0f);
      glTexCoord2f(0.0f, 1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }
  if(textures[1] && checked)
  {
    textures[1]->render_gl();
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBegin(GL_TRIANGLE_STRIP);
      glVertex3f(pos[0], pos[1], 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(pos[0] + dim[0], pos[1], 0.0f);
      glTexCoord2f(0.0f, 0.0f);

      glVertex3f(pos[0], pos[1] - dim[1], 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(pos[0] + dim[0], pos[1] - dim[1], 0.0f);
      glTexCoord2f(0.0f, 1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
  }
}
