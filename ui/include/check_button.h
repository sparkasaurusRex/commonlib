#ifndef __CHECK_BUTTON_H__
#define __CHECK_BUTTON_H__

#include "push_button.h"

namespace UI
{
  class CheckButton : public PushButton
  {
  public:
    CheckButton() : PushButton() { checked = false; }
    ~CheckButton() {}

    bool get_state() const { return checked; }
    void set_state(const bool s) { checked = s; }

    virtual void process_event(const SDL_Event &event);
    virtual void render();
  protected:
    bool checked;
  };
};

#endif //__CHECK_BUTTON_H__
