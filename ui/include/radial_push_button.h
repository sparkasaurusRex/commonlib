#ifndef __RADIAL_PUSH_BUTTON_H__
#define __RADIAL_PUSH_BUTTON_H__

#include "texture.h"
#include "radial_widget.h"

namespace UI
{
  class RadialPushButton : public RadialWidget
  {
  public:
    RadialPushButton(Font *f = NULL);
    ~RadialPushButton() {}

    void set_click_callback(void (*cb)(const SDL_Event &e));
    void set_texture(const int i, Texture *t);
    void set_click_color(const Float3 c) { click_rgb = c; }

    virtual void init();
    virtual void process_event(const SDL_Event &event);
    virtual void render();
  protected:
    void (*click_callback)(const SDL_Event &e); //callback func
    Float3 click_rgb;
    bool click_capture;
    Texture *textures[2]; //2 to switch textures upon click
  };
};

#endif //__RADIAL_PUSH_BUTTON_H__
