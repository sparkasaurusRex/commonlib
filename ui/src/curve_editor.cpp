#include "curve_editor.h"

using namespace UI;

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

  glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);

    glVertex3f(pos[0], pos[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1], 0.0f);
    glVertex3f(pos[0] + dim[0], pos[1] + dim[1], 0.0f);
    glVertex3f(pos[0], pos[1] + dim[1], 0.0f);
    glVertex3f(pos[0], pos[1], 0.0f);
  glEnd();
}

void CurveEditor::process_event(const SDL_Event &e)
{

}

void CurveEditor::add_control_point(const float x)
{

}

void CurveEditor::delete_control_point()
{

}
