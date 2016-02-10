#include <assert.h>
#include "curve_editor.h"

using namespace UI;
using namespace Math;
using namespace std;

CurveEditor::CurveEditor(Font *f) : RectangularWidget(f)
{
  curve = NULL;
  selected = NULL;
}

CurveEditor::~CurveEditor()
{

}

void CurveEditor::init()
{

}

void CurveEditor::render()
{
  //TODO: programmable pipeline?

  //draw the border
  glLineWidth(2.0f);
  glBegin(GL_LINE_STRIP);
    glColor3f(0.5f, 0.5f, 0.5f);

    glVertex3f(pos[0], pos[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1] + dim[1], 0.0f);
    glVertex3f(pos[0], pos[1] + dim[1], 0.0f);
    glVertex3f(pos[0], pos[1], 0.0f);
  glEnd();

  //draw the curve
  if(curve)
  {
    int num_segments = 32;
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    for(int i = 0; i <= num_segments; i++)
    {
      float x_pct = (float)i / (float)num_segments;
      CurveSegment *cs = curve->get_segment(x_pct);
      if(cs)
      {

        float y = 1.0f - cs->evaluate(x_pct);

        float screen_x = pos[0] + x_pct * dim[0];
        float screen_y = pos[1] + y * dim[1];
        glVertex3f(screen_x, screen_y, 0.0f);
      }
    }
    glEnd();

    //endpoints
    int num_curve_segments = curve->get_num_segments();
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);
    for(int i = 0; i < num_curve_segments; i++)
    {
      CurveSegment *cs = curve->get_segment(i);
      float screen_x = pos[0] + cs->end_points[0].p[0] * dim[0];
      float screen_y = pos[1] + (1.0f - cs->end_points[0].p[1]) * dim[1];
      glVertex3f(screen_x, screen_y, 0.0f);

      screen_x = pos[0] + cs->end_points[1].p[0] * dim[0];
      screen_y = pos[1] + (1.0f - cs->end_points[1].p[1]) * dim[1];
      glVertex3f(screen_x, screen_y, 0.0f);
    }
    glEnd();
  }
}

void CurveEditor::process_event(const SDL_Event &event)
{
  assert(curve);

  if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT)
  {
    //click capture, etc...
    int mouse_x, mouse_y;
    Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);

    //if(hit_test(mouse_x, mouse_y))
    {
      float fx = ((float)mouse_x - pos[0]) / dim[0];
      float fy = 1.0f - ((float)mouse_y - pos[1]) / dim[1];

      float curve_y = curve->evaluate(fx);
      if(fabs(curve_y - fy) < 0.05f)
      {
        cout<<"hit!"<<endl;
        CurveSegment *cs = curve->get_segment(fx);
        assert(cs);

        float new_x = cs->end_points[1].p[0];
        float new_y = cs->end_points[1].p[1];

        cs->end_points[1].p[0] = fx;
        cs->end_points[1].p[1] = curve_y;

        CurveSegment *new_seg = curve->create_segment(INTERPOLATE_LERP, Float2(fx, new_x));
        assert(new_seg);
        new_seg->end_points[0].p[1] = curve_y;
        new_seg->end_points[1].p[1] = new_y;
      }
    }
  }
}

void CurveEditor::add_control_point(const float x)
{

}

void CurveEditor::delete_control_point()
{

}
