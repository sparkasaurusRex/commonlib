#ifndef __WIDGET_WRANGLER_H__
#define __WIDGET_WRANGLER_H__

#include <vector>
#include "widget.h"

namespace UI
{
  class WidgetWrangler
  {
  public:
    WidgetWrangler() {}
    ~WidgetWrangler() {}

    void add_widget(Widget *w) { widgets.push_back(w); }
    void render() const;
  private:
    std::vector<Widget *> widgets;
  };
};

#endif //__WIDGET_WRANGLER_H__
