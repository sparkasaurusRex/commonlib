#ifndef __PUSH_BUTTON_H__
#define __PUSH_BUTTON_H__

#include "texture.h"
#include "label.h"

namespace UI
{
  class PushButton : public Label
  {
  public:
    PushButton();
    ~PushButton() {}

    void set_click_callback(void (*cb)(const SDL_Event &e));
    void set_texture(const int i, Texture *t);
    void set_click_color(const Float3 c) { click_rgb = c; }

    //void init();
    virtual void process_event(const SDL_Event &event);
    virtual void render();
  protected:
    void (*click_callback)(const SDL_Event &e); //callback func
    bool click_capture;
    Texture *textures[2]; //2 to switch textures upon click
    Float3 click_rgb;
  };
};
#endif //__PUSH_BUTTON_H__
