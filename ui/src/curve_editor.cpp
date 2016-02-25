#include <assert.h>
#include "curve_editor.h"

using namespace UI;
using namespace Math;
using namespace std;

CurveEditor::CurveEditor(Font *f) : RectangularWidget(f)
{
  curve = NULL;
  selected_handle = NULL;
  selected_segment = NULL;
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

  //draw grid lines
  int num_grid_lines = 10;
  glLineWidth(1.0f);
  glBegin(GL_LINES);
  glColor3f(0.2f, 0.2f, 0.2f);
  for(int i = 1; i < num_grid_lines; i++)
  {
    float val = (float)(i % (num_grid_lines / 4)) / (float)(num_grid_lines / 4);
    val = 0.2f + val / 3.0f;
    glColor3f(val, val, val);

    float sx = pos[0] + ((float)i / (float)num_grid_lines) * dim[0];
    float sy = pos[1] + ((float)i / (float)num_grid_lines) * dim[1];

    //horizontal
    glVertex3f(pos[0], sy, 0.0f);
    glVertex3f(pos[0] + dim[0], sy, 0.0f);

    //vertical
    glVertex3f(sx, pos[1], 0.0f);
    glVertex3f(sx, pos[1] + dim[1], 0.0f);
  }
  glEnd();

  //draw the curve
  if(curve)
  {
    int num_segments = CURVE_EDITOR_NUM_DRAW_SEGMENTS;
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i <= num_segments; i++)
    {
      float x_pct = (float)i / (float)num_segments;
      CurveSegment *cs = curve->get_segment(x_pct);
      if(cs)
      {
        if(cs == selected_segment)
        {
          glLineWidth(3.0f);
          glColor3f(1.0f, 0.0f, 0.0f);
        }
        else
        {
          glLineWidth(1.0f);
          glColor3f(1.0f, 1.0f, 1.0f);
        }

        float y = 1.0f - cs->evaluate(x_pct);

        float screen_x = pos[0] + x_pct * dim[0];
        float screen_y = pos[1] + y * dim[1];
        glVertex3f(screen_x, screen_y, 0.0f);
      }
    }
    glEnd();

    //endpoints / tangent points
    int num_curve_segments = curve->get_num_segments();
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for(int i = 0; i < num_curve_segments; i++)
    {
      CurveSegment *cs = curve->get_segment_by_index(i);
      for(int j = 0; j < 2; j++)
      {
        float screen_x = pos[0] + cs->end_points[j].p[0] * dim[0];
        float screen_y = pos[1] + (1.0f - cs->end_points[j].p[1]) * dim[1];
        /*if(&(cs->end_points[j]) == selected_endpt)
        {
          glColor3f(1.0f, 1.0f, 0.0f);
        }
        else
        {*/
          glColor3f(1.0f, 0.0f, 0.0f);
        //}
        glVertex3f(screen_x, screen_y, 0.0f);
      }

      if(cs->get_interpolation_method() == INTERPOLATE_BEZIER)
      {
        for(int j = 0; j < 2; j++)
        {
          if(true)//selected_endpt == &(cs->end_points[j]) || selected_endpt == cs->end_points[j].neighbor)
          {
            float screen_x = pos[0] + cs->end_points[j].t[0] * dim[0];
            float screen_y = pos[1] + (1.0f - cs->end_points[j].t[1]) * dim[1];
            glColor3f(0.0f, 1.0f, 1.0f);
            glVertex3f(screen_x, screen_y, 0.0f);
          }
        }
      }
    }
    glEnd();

    //lines from end point to bezier control points (tangents)
    glBegin(GL_LINES);
    glColor3f(0.6f, 0.6f, 0.6f);
    for(int i = 0; i < num_curve_segments; i++)
    {
      CurveSegment *cs = curve->get_segment_by_index(i);
      if(cs->get_interpolation_method() == INTERPOLATE_BEZIER)
      {
        for(int j = 0; j < 2; j++)
        {
          if(true) //selected_endpt == &(cs->end_points[j]) || selected_endpt == cs->end_points[j].neighbor)
          {
            float screen_x = pos[0] + cs->end_points[j].p[0] * dim[0];
            float screen_y = pos[1] + (1.0f - cs->end_points[j].p[1]) * dim[1];
            glVertex3f(screen_x, screen_y, 0.0f);

            screen_x = pos[0] + cs->end_points[j].t[0] * dim[0];
            screen_y = pos[1] + (1.0f - cs->end_points[j].t[1]) * dim[1];
            glVertex3f(screen_x, screen_y, 0.0f);
          }
        }
      }
    }
    glEnd();
  }
}

void CurveEditor::process_event(const SDL_Event &event)
{
  assert(curve);

  //cout<<"CurveEditor::process_event"<<endl;

  if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
  {
    int mouse_x, mouse_y;
    Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);

    float fx = ((float)mouse_x - pos[0]) / dim[0];
    float fy = 1.0f - ((float)mouse_y - pos[1]) / dim[1];
    select_control_point(fx, fy);
    select_segment(fx, fy);
  }

  if(event.type == SDL_MOUSEMOTION)
  {
    int mouse_x, mouse_y;
    Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    if(button_state & SDL_BUTTON_LEFT)
    {
      float fx = ((float)mouse_x - pos[0]) / dim[0];
      float fy = 1.0f - ((float)mouse_y - pos[1]) / dim[1];

      move_selected_control_point(fx, fy);
    }
  }

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
      if(fabs(curve_y - fy) < CURVE_EDITOR_CLICK_THRESHOLD)
      {
        add_control_point(fx);
      }
    }
  }
}

void CurveEditor::add_control_point(const float fx)
{
  CurveSegment *cs = curve->get_segment(fx);
  assert(cs);

  Float2 new_curve_pt(fx, curve->evaluate(fx));

  CurveEndPoint middle, right;

  middle.p = new_curve_pt;
  middle.t = new_curve_pt + Float2(0.1f, 0.0f);

  right.p = cs->end_points[1].p;
  right.t = cs->end_points[1].t;

  CurveSegment *new_segment = curve->create_segment(INTERPOLATE_BEZIER, middle, right);
  cs->end_points[1].p = middle.p;
  cs->end_points[1].t = middle.p + Float2(-0.1f, 0.0f);

  for(int i = 0; i < curve->get_num_segments(); i++)
  {
    CurveSegment *cs = curve->get_segment_by_index(i);
    cout<<"segment"<<endl;
    cout<<"\t"<<cs->end_points[0].p<<endl;
    cout<<"\t"<<cs->end_points[1].p<<endl;
  }
}

void CurveEditor::delete_control_point()
{

}

void CurveEditor::select_control_point(const float fx, const float fy)
{
  selected_handle = NULL;

  int num_curve_handles = curve->get_num_handles();
  for(int i = 0; i < num_curve_handles; i++)
  {
    CurveHandle *ch = curve->get_handle_by_index(i);
    assert(ch->locations.size() > 0);
    Float2 hp = *(ch->locations[0]);

    if(fabs(hp[0] - fx) < CURVE_EDITOR_CLICK_THRESHOLD &&
       fabs(hp[1] - fy) < CURVE_EDITOR_CLICK_THRESHOLD)
    {
      selected_handle = ch;
    }
  }
}

void CurveEditor::select_segment(const float fx, const float fy)
{
  selected_segment = NULL;
  CurveSegment *cs = curve->get_segment(fx);
  if(cs)
  {
    float cy = cs->evaluate(fx);
    cout<<"cy: "<<cy<<endl;
    cout<<"fy: "<<fy<<endl;
    if(fabs(fy - cy) < CURVE_EDITOR_CLICK_THRESHOLD)
    {
      selected_segment = cs;
    }
  }
}

void CurveEditor::move_selected_control_point(const float fx, const float fy)
{
  if(selected_handle)
  {
    selected_handle->translate(Math::Float2(fx, fy));
    curve->enforce_segment_ranges();
  }
}
