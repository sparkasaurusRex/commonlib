#ifndef __CHECK_BUTTON_H__
#define __CHECK_BUTTON_H__

#include "push_button.h"

namespace UI
{
  class RadioGroup;
  class CheckButton : public PushButton
  {
  public:
    CheckButton() : PushButton() { checked = false; radio_group = NULL; }
    ~CheckButton() {}

    bool get_state() const { return checked; }
    void set_state(const bool s) { checked = s; }

    void set_radio_group(RadioGroup *rg) { radio_group = rg; }
    RadioGroup *get_radio_group() { return radio_group; }

    virtual void process_event(const SDL_Event &event, const Math::Float2 offset = Math::Float2());
    virtual void render();
  protected:
    bool checked;
    RadioGroup *radio_group;
  };
};

#endif //__CHECK_BUTTON_H__
