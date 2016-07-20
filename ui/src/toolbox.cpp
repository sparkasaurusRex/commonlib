#include <iostream>
#include "toolbox.h"

using namespace UI;
using namespace std;
using namespace Math;

ToolBox::ToolBox() : RectangularWidget(WIDGET_TOOLBOX)
{
  horizontal = true;    //horizontal alignment by default
  margin = 4.0f;        //5 pixels betwixt each button
  pb_dim[0] = pb_dim[1] = 32;
}

void ToolBox::init()
{
  //dim[0] = margin * (buttons.size() + 1) + buttons.size() * pb_dim[0];
  //dim[1] = margin * 2 + pb_dim[1];

  float dx = margin;
  for(unsigned int i = 0; i < buttons.size(); i++)
  {
    Float2 pb_pos;
    if(horizontal) { pb_pos = Float2(dx, margin); }
    else { pb_pos = Float2(margin, dx); }
    buttons[i]->translate(pb_pos + pos);
    buttons[i]->scale(pb_dim);
    if(horizontal) { dx += margin + pb_dim[0]; }
    else { dx += margin + pb_dim[1]; }
  }

  if(horizontal)
  {
    dim[0] = dx;
    dim[1] = 2.0f * margin + pb_dim[1];
  }
  else
  {
    dim[0] = 2.0f * margin + pb_dim[0];
    dim[1] = dx;
  }
  //cout<<"dim: "<<dim<<endl;
}

void ToolBox::show(const bool d)
{
  for(unsigned int i = 0; i < buttons.size(); i++)
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
    for(unsigned int i = 0; i < buttons.size(); i++)
    {
      buttons[i]->render();
    }
    //glPopMatrix();
  }
}

void ToolBox::process_event(const SDL_Event &event, const Math::Float2 offset)
{
  //std::cout<<"meow"<<std::endl;
  //SDL_Event event_copy = event;
  //event_copy.
  for(unsigned int i = 0; i < buttons.size(); i++)
  {
    buttons[i]->process_event(event, offset);
  }

  //for(unsigned int)
}
