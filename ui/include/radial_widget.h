#ifndef __RADIAL_WIDGET_H__
#define __RADIAL_WIDGET_H__

#include "widget.h"

namespace UI
{
  class RadialWidget : public Widget
  {
  public:
    RadialWidget(Font *f = NULL) : Widget(f) {}
    ~RadialWidget() {}

    virtual bool hit_test(const int x, const int y) const;
  protected:
    Math::Float2 radii;
    Math::Float2 arc;
    Math::Float2 center;
  };
};

#endif //__RADIAL_WIDGET_H__
