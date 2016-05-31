#ifndef __CHECK_BUTTON_H__
#define __CHECK_BUTTON_H__

#include "push_button.h"

namespace UI
{
  class CheckButton : public PushButton
  {
  public:
    CheckButton() : PushButton() { checked = false; radio = false; }
    ~CheckButton() {}

    bool get_state() const { return checked; }
    void set_state(const bool s) { checked = s; }

    bool is_radio() const { return radio; }
    void set_radio(const bool r) { radio = r; }

    virtual void process_event(const SDL_Event &event, const Math::Float2 offset = Math::Float2());
    virtual void render();
  protected:
    bool checked;
    bool radio;
  };
};

#endif //__CHECK_BUTTON_H__
