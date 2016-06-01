#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <iostream>

#if defined(__APPLE__)
#include <OpenGL/glu.h>
#endif

#if defined(_WIN32)
#include <Windows.h>
#include <GL/glew.h>
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif //_WIN32

#include "math_utility.h"
#include "font.h"

namespace UI
{
  class Widget
  {
  public:
    Widget(Font *f = NULL);
    ~Widget() {}

    virtual void show(const bool d = true) { visible = d; }
    bool is_visible() const { return visible; }
    void set_font(Font *f) { font = f; }

    virtual bool hit_test(const int x, const int y) const = 0;

    virtual void process_event(const SDL_Event &e, const Math::Float2 offset = Math::Float2()) = 0;

    virtual void init() = 0;
    virtual void simulate(const double game_time, const double frame_time);
    virtual void render() = 0;

    void set_focus(bool f) { has_focus = f; }
    bool get_focus() const { return has_focus; }

    void set_tooltip(const std::string tt) { tooltip = tt; has_tooltip = true; }
    std::string get_tooltip() const { return tooltip; }
    virtual void render_tooltip() = 0;

    void set_hovering(const bool h) { hovering = h; }
    void enable(const bool e) { enabled = e; }

  protected:
    void render_prep();
    void render_cleanup();

    bool visible;
    Font *font;

    bool has_focus;

    bool has_tooltip;
    std::string tooltip;
    Math::Float2 tooltip_offset;
    
    bool hovering;
    float hover_timer;

    bool enabled;
  };
};

#endif //__WIDGET_H__
