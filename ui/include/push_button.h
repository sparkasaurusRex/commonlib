#ifndef __PUSH_BUTTON_H__
#define __PUSH_BUTTON_H__

#include "label.h"

namespace UI
{
  class PushButton : public Label
  {
  public:
    PushButton() : Label() { click_capture = false; }
    ~PushButton() {}

    void process_event(const SDL_Event &event);
    void render();
  protected:
    bool click_capture;
    //Texture *idle_tex;
  };
};
#endif //__PUSH_BUTTON_H__
