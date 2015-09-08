#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "math_utility.h"
#include "font.h"

namespace UI
{
  class Widget
  {
  public:
    Widget(Font *f = NULL) { font = f; visible = false; }
    ~Widget() {}

    void show(const bool d = true) { visible = d; }
    void set_font(Font *f) { font = f; }
    void translate(Math::Float2 p) { pos = p; }

    virtual void init() = 0;
    virtual void simulate(const float dt) = 0;
    virtual void render() = 0;
  protected:

    void render_prep();
    void render_cleanup();

    bool visible;
    Font *font;
    Math::Float2 pos;
  };
};

#endif //__MCGUFFIN_H__
