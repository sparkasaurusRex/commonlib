#ifndef __CURVE_EDITOR_H__
#define __CURVE_EDITOR_H__

#include "rectangular_widget.h"
#include "curve.h"

namespace UI
{
  class CurveEditor : public RectangularWidget
  {
  public:
    CurveEditor(Font *f = NULL);
    ~CurveEditor();

    virtual void init();
    virtual void simulate(const float dt) {}
    virtual void render();

    virtual void process_event(const SDL_Event &e);

    void set_curve(Math::Curve *c) { curve = c; }

    void add_control_point(const float x);
    void delete_control_point();

    void grab_control_point();
    void release_control_point();
  private:

    //curve data
    Math::Curve *curve;
    Math::CurveEndPoint *selected;

    //curve rendering
    struct CurveVert
    {
      float x, y, z;
    };
    GLuint curve_vbo;
    GLuint curve_ibo;
    CurveVert *curve_verts;
    GLuint *curve_indices;
  };
};

#endif //__CURVE_EDITOR_H__
