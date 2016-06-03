#ifndef __RADIAL_WIDGET_H__
#define __RADIAL_WIDGET_H__

#include "widget.h"

namespace UI
{
  class RadialWidget : public Widget
  {
  protected:
    Math::Float2 radii;
    Math::Float2 arc;
    Math::Float2 center;
  public:
    RadialWidget(Graphics::Font *f = NULL) : Widget(f) {}
    ~RadialWidget() {}

    void set_radii(const Math::Float2 r) { radii = r; }
    void set_arc(const Math::Float2 a) { arc = a; }
    void set_center(const Math::Float2 c) { center = c; }

    virtual bool hit_test(const int x, const int y) const;
    virtual void simulate(const float dt) {}

    virtual void render_tooltip();
  };
};

#endif //__RADIAL_WIDGET_H__
