#ifndef __TEXT_EDIT_H__
#define __TEXT_EDIT_H__

#include "rectangular_widget.h"

namespace UI
{
  class TextEdit : public RectangularWidget
  {
  public:
    TextEdit(Graphics::Font *f = NULL);
    ~TextEdit() {}

    virtual void process_event(const SDL_Event &e, const Math::Float2 offset = Math::Float2());
    virtual void init();
    virtual void simulate(const double game_time, const double frame_time);
    virtual void render();

    std::string get_text() const { return text; }
    void set_text(const std::string t) { text = t; }
  protected:
    std::string text;
    int selection[2];
  };
};

#endif //__TEXT_EDIT_H__
