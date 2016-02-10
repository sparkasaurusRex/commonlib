#include <assert.h>
#include "curve_editor.h"

using namespace UI;
using namespace Math;
using namespace std;

CurveEditor::CurveEditor(Font *f) : RectangularWidget(f)
{
  curve = NULL;
  selected_endpt = NULL;
  tangent_selected = false;
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
        if(&(cs->end_points[j]) == selected_endpt)
        {
          glColor3f(1.0f, 1.0f, 0.0f);
        }
        else
        {
          glColor3f(1.0f, 0.0f, 0.0f);
        }
        glVertex3f(screen_x, screen_y, 0.0f);
      }

      if(cs->get_interpolation_method() == INTERPOLATE_BEZIER)
      {
        for(int j = 0; j < 2; j++)
        {
          float screen_x = pos[0] + cs->end_points[j].t[0] * dim[0];
          float screen_y = pos[1] + (1.0f - cs->end_points[j].t[1]) * dim[1];
          glColor3f(0.0f, 1.0f, 1.0f);
          glVertex3f(screen_x, screen_y, 0.0f);
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
          float screen_x = pos[0] + cs->end_points[j].p[0] * dim[0];
          float screen_y = pos[1] + (1.0f - cs->end_points[j].p[1]) * dim[1];
          glVertex3f(screen_x, screen_y, 0.0f);

          screen_x = pos[0] + cs->end_points[j].t[0] * dim[0];
          screen_y = pos[1] + (1.0f - cs->end_points[j].t[1]) * dim[1];
          glVertex3f(screen_x, screen_y, 0.0f);
        }
      }
    }
    glEnd();
  }
}

void CurveEditor::process_event(const SDL_Event &event)
{
  assert(curve);

  if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
  {
    int mouse_x, mouse_y;
    Uint32 button_state = SDL_GetMouseState(&mouse_x, &mouse_y);

    float fx = ((float)mouse_x - pos[0]) / dim[0];
    float fy = 1.0f - ((float)mouse_y - pos[1]) / dim[1];
    select_control_point(fx, fy);
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
        cout<<"hit!"<<endl;
        add_control_point(fx);
      }
    }
  }
}

void CurveEditor::add_control_point(const float fx)
{
  float curve_y = curve->evaluate(fx);

  CurveSegment *cs = curve->get_segment(fx);
  assert(cs);

  float new_x = cs->end_points[1].p[0];
  float new_y = cs->end_points[1].p[1];

  cs->end_points[1].p[0] = fx;
  cs->end_points[1].p[1] = curve_y;

  CurveSegment *new_seg = curve->create_segment(INTERPOLATE_BEZIER, Float2(fx, new_x));
  assert(new_seg);
  new_seg->end_points[0].p[1] = curve_y;
  new_seg->end_points[1].p[1] = new_y;

  new_seg->end_points[0].t[0] = fx - 0.2f;
  new_seg->end_points[0].t[1] = curve_y;

  new_seg->end_points[1].t[0] = new_seg->end_points[1].p[0] - 0.2f;
  new_seg->end_points[1].t[1] = new_y;

  new_seg->end_points[0].neighbor = &(cs->end_points[1]);
  cs->end_points[1].neighbor = &(new_seg->end_points[0]);
}

void CurveEditor::delete_control_point()
{

}

void CurveEditor::select_control_point(const float fx, const float fy)
{
  selected_endpt = NULL;
  tangent_selected = false;
  int num_curve_segments = curve->get_num_segments();
  for(int ci = 0; ci < num_curve_segments; ci++)
  {
    CurveSegment *cs = curve->get_segment_by_index(ci);
    for(int i = 0; i < 2; i++)
    {
      if(fabs(cs->end_points[i].p[0] - fx) < CURVE_EDITOR_CLICK_THRESHOLD &&
         fabs(cs->end_points[i].p[1] - fy) < CURVE_EDITOR_CLICK_THRESHOLD)
       {
         selected_endpt = &(cs->end_points[i]);
         tangent_selected = false;
       }
       else if(fabs(cs->end_points[i].t[0] - fx) < CURVE_EDITOR_CLICK_THRESHOLD &&
               fabs(cs->end_points[i].t[1] - fy) < CURVE_EDITOR_CLICK_THRESHOLD)
       {
         selected_endpt = &(cs->end_points[i]);
         tangent_selected = true;
       }
     }
   }
}

void CurveEditor::move_selected_control_point(const float fx, const float fy)
{
  //ugh this code is sooooo ugly....
  if(selected_endpt)
  {
    Float2 *handle = NULL;
    if(tangent_selected)
    {
      handle = &(selected_endpt->t);
    }
    else
    {
      handle = &(selected_endpt->p);
    }
    (*handle)[0] = fx;
    (*handle)[1] = fy;

    if(selected_endpt->neighbor)
    {
      selected_endpt->neighbor->p[0] = fx;
      selected_endpt->neighbor->p[1] = fy;
    }
  }
}
