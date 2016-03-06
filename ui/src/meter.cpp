#include "meter.h"

using namespace UI;
using namespace std;

Meter::Meter(Font *f) : RectangularWidget(f)
{
  pct = 0.0f;
}

void Meter::render()
{
  if(visible)
  {

    //draw background
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLE_STRIP);
      glVertex3f(pos[0], pos[1], 0.0f);
      glVertex3f(pos[0] + dim[0], pos[1], 0.0f);

      glVertex3f(pos[0], pos[1] - dim[1], 0.0f);
      glVertex3f(pos[0] + dim[0], pos[1] - dim[1], 0.0f);
    glEnd();

    //draw meter
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_STRIP);
      glVertex3f(pos[0], pos[1], 0.0f);
      glVertex3f(pos[0] + dim[0] * pct, pos[1], 0.0f);

      glVertex3f(pos[0], pos[1] - dim[1], 0.0f);
      glVertex3f(pos[0] + dim[0] * pct, pos[1] - dim[1], 0.0f);
    glEnd();
  }
}
