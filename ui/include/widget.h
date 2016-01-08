#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <iostream>
#include <SDL2/SDL.h>

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
    bool is_visible() const { return visible; }
    void set_font(Font *f) { font = f; }

    virtual bool hit_test(const int x, const int y) const = 0;

    virtual void process_event(const SDL_Event &e) = 0;

    virtual void init() = 0;
    virtual void simulate(const float dt) = 0;
    virtual void render() = 0;

  protected:
    void render_prep();
    void render_cleanup();

    bool visible;
    Font *font;
  };
};

#endif //__MCGUFFIN_H__
