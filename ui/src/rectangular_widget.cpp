#include "rectangular_widget.h"

#include <iostream>

using namespace UI;
using namespace std;

bool RectangularWidget::hit_test(const int x, const int y) const
{
  bool hit =  ((x > pos[0]) && (x < (pos[0] + dim[0])) &&
          (y < pos[1]) && (y > (pos[1] - dim[1])));

  /*if(hit)
  {
    cout<<"hit: "<<pos<<endl;
  }*/
  return hit;
}

void RectangularWidget::render_tooltip()
{
  //calculate size
  char buffer[1024];
  strcpy(buffer, tooltip.c_str());
  float w = font->get_string_width(buffer);
  float h = font->get_height();

  glEnable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);

  //render background box
  glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
  glBegin(GL_TRIANGLE_STRIP);
    //glTexCoord2f(0.0f, 1.0f);
    glVertex3f(pos[0] + tooltip_offset[0], pos[1] + tooltip_offset[1], 0.0f);
    //glTexCoord2f(1.0f, 1.0f);
    glVertex3f(pos[0] + tooltip_offset[0] + w, pos[1] + tooltip_offset[1], 0.0f);

    //glTexCoord2f(0.0f, 0.0f);
    glVertex3f(pos[0] + tooltip_offset[0], pos[1] + tooltip_offset[1] - h, 0.0f);
    //glTexCoord2f(1.0f, 0.0f);
    glVertex3f(pos[0] + tooltip_offset[0] + w, pos[1] + tooltip_offset[1] - h, 0.0f);
  glEnd();

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  //render text
  font->print(pos[0] + tooltip_offset[0], pos[1] + tooltip_offset[1], tooltip.c_str());
}