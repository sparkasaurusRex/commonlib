#ifndef __RECTANGULAR_WIDGET_H__
#define __RECTANGULAR_WIDGET_H__

#include "widget.h"

namespace UI
{
  class RectangularWidget : public Widget
  {
  protected:
    Math::Float2 pos;
    Math::Float2 dim;
  public:
    RectangularWidget(Font *f = NULL) : Widget(f) {}
    ~RectangularWidget() {}

    virtual bool hit_test(const int x, const int y) const;

    void translate(Math::Float2 p) { pos = p; }
    void scale(Math::Float2 s) { dim = s; }
    Math::Float2 get_dim() const { return dim; }
    Math::Float2 get_pos() const { return pos; }

    virtual void process_event(const SDL_Event &e, const Math::Float2 offset = Math::Float2()) = 0;

    virtual void init() = 0;
    virtual void simulate(const double game_time, const double frame_time) { Widget::simulate(game_time, frame_time); }
    virtual void render() = 0;
    virtual void render_tooltip();
  };
};

#endif //__RECTANGULAR_WIDGET_H__
