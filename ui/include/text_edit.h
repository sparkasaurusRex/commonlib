#ifndef __TEXT_EDIT_H__
#define __TEXT_EDIT_H__

#include "rectangular_widget.h"

namespace UI
{
  class TextEdit : public RectangularWidget
  {
  public:
    TextEdit(Font *f = NULL);
    ~TextEdit() {}

    virtual void process_event(const SDL_Event &e);
    virtual void init();
    virtual void simulate(const float dt);
    virtual void render();

    std::string get_text() const { return text; }
  protected:
    std::string text;
    int selection[2];
  };
};

#endif //__TEXT_EDIT_H__
