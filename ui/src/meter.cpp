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


void Meter::process_event(const SDL_Event &e)
{
  //cout << "Meter::process_event()" << endl;
  if (e.type == SDL_MOUSEMOTION && (e.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)))
  {
    int mouse_x, mouse_y;
    Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    if (hit_test(mouse_x, mouse_y))
    {
      float pct = (mouse_x - pos[0]) / dim[0];
      set_percent(pct);
    }
  }
}
