#ifndef __LABEL_H__
#define __LABEL_H__

#include "math_utility.h"
#include "rectangular_widget.h"

namespace UI
{
  class Label : public RectangularWidget
  {
  public:
    Label(Font *f = NULL);
    ~Label() {}

    void set_text(const std::string t) { text = t; }
    void set_color(const Math::Float3 c) { rgb = c; }

    virtual void process_event(const SDL_Event &e, const Math::Float2 offset = Math::Float2()) {}

    virtual void init() {}
    virtual void simulate(const float dt) {}
    virtual void render();
  protected:
    std::string text;
    Math::Float3 rgb;
  };
};

#endif //__LABEL_H__
