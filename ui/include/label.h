#ifndef __LABEL_H__
#define __LABEL_H__

#include "math_utility.h"
#include "widget.h"

namespace UI
{
  class Label : public Widget
  {
  public:
    Label(Font *f = NULL);
    ~Label() {}

    void set_text(const std::string t) { text = t; }
    void set_color(const Math::Float3 c) { rgb = c; }

    void init() { }
    void simulate(const float dt) {}
    void render();
  protected:
    std::string text;
    Math::Float3 rgb;
  };
};

#endif //__LABEL_H__
