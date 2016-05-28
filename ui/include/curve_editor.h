#ifndef __CURVE_EDITOR_H__
#define __CURVE_EDITOR_H__

#include "rectangular_widget.h"
#include "text_edit.h"

#include "curve.h"

#define CURVE_EDITOR_NUM_DRAW_SEGMENTS 128
#define CURVE_EDITOR_CLICK_THRESHOLD 0.05f

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

    virtual void process_event(const SDL_Event &e, const Math::Float2 offset = Math::Float2());

    void set_curve(Math::Curve *c) { curve = c; }

    void add_control_point(const float x);
    void delete_control_point();

    void select_control_point(const float x, const float y);
    void select_segment(const float x, const float y);
    void move_selected_control_point(const float x, const float y);

    void grab_control_point();
    void release_control_point();
  private:

    //curve data
    Math::Curve *curve;
    Math::CurveHandle     *selected_handle;
    Math::CurveHandle     *last_selected_handle;
    Math::CurveSegment    *selected_segment;

    TextEdit handle_pos_te[2];

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
