#ifndef __WIDGET_WRANGLER_H__
#define __WIDGET_WRANGLER_H__

#include <vector>
#include "widget.h"

namespace UI
{
  class WidgetWrangler
  {
  public:
    WidgetWrangler();
    ~WidgetWrangler() {}

    void add_widget(Widget *w) { widgets.push_back(w); }
    void render() const;
    void process_event(const SDL_Event &e);
    void simulate(const float dt);
  private:
    std::vector<Widget *> widgets;
    int focus_idx;
  };
};

#endif //__WIDGET_WRANGLER_H__
