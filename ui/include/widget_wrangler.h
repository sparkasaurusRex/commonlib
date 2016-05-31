#ifndef __WIDGET_WRANGLER_H__
#define __WIDGET_WRANGLER_H__

#include <vector>
#include "widget.h"
#include "math_utility.h"

namespace UI
{
  class WidgetWrangler
  {
  public:
    WidgetWrangler();
    ~WidgetWrangler() {}

    void add_widget(Widget *w) { widgets.push_back(w); }
    void render(const Math::Float3 offset = Math::Float3()) const;
    void process_event(const SDL_Event &e, const Math::Float2 offset = Math::Float2());
    void simulate(const double game_time, const double frame_time);
  private:
    std::vector<Widget *> widgets;
    int focus_idx;
  };
};

#endif //__WIDGET_WRANGLER_H__
