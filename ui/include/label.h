#ifndef __LABEL_H__
#define __LABEL_H__

#include "widget.h"

namespace UI
{
  class Label : public Widget
  {
  public:
    Label(Font *f = NULL) : Widget(f) { text = std::string("Label Text"); }
    ~Label() {}

    void set_text(std::string t) { text = t; }

    void init() { }
    void simulate(const float dt) {}
    void render();
  protected:
    std::string text;
  };
};

#endif //__LABEL_H__
