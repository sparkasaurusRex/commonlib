#include <iostream>
#include "toolbox.h"

using namespace UI;

ToolBox::ToolBox() : Widget()
{
  horizontal = true;    //horizontal alignment by default
  margin = 4.0f;        //5 pixels betwixt each button
  pb_dim[0] = pb_dim[1] = 32;
}

void ToolBox::init()
{
  dim[0] = margin * (buttons.size() + 1) + buttons.size() * pb_dim[0];
  dim[1] = margin * 2 + pb_dim[1];

  float dx = margin;
  for(int i = 0; i < buttons.size(); i++)
  {
    Float2 pb_pos(dx, margin);
    buttons[i]->translate(pb_pos + pos);
    buttons[i]->scale(pb_dim);
    dx += margin + pb_dim[0];
  }
}

void ToolBox::show(const bool d)
{
  for(int i = 0; i < buttons.size(); i++)
  {
    buttons[i]->show(d);
  }
  visible = d;
}

void ToolBox::translate(Float2 p)
{
  pos = p;
  ToolBox::init();
}

void ToolBox::render()
{
  if(visible)
  {
    //draw a box
    //glMatrixMode(GL_MODELVIEW_MATRIX);
    //glPushMatrix();
    //glTranslatef(pos[0], pos[1], 0.0f);
    for(int i = 0; i < buttons.size(); i++)
    {
      buttons[i]->render();
    }
    //glPopMatrix();
  }
}

void ToolBox::process_event(const SDL_Event &event)
{
  //std::cout<<"meow"<<std::endl;
  //SDL_Event event_copy = event;
  //event_copy.
  for(int i = 0; i < buttons.size(); i++)
  {
    buttons[i]->process_event(event);
  }
}
