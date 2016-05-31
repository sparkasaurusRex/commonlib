#ifndef __PUSH_BUTTON_H__
#define __PUSH_BUTTON_H__

#include "texture.h"
#include "label.h"

namespace UI
{
  class PushButton : public Label
  {
  protected:
    bool                  click_capture; //true if we are clicking 
    Graphics::Texture2D   *textures[3];  // 3 to switch textures upon click
    Math::Float3          click_rgb;     // tint color while button is clicked
  public:
    PushButton(Font *f = NULL);
    ~PushButton() {}

    void set_click_callback(void (*cb)(const SDL_Event &e));
    void set_texture(const int i, Graphics::Texture2D *t);
    void set_click_color(const Math::Float3 c) { click_rgb = c; }

    //void init();
    virtual void process_event(const SDL_Event &event, const Math::Float2 offset = Math::Float2());
    virtual void render();
    virtual void simulate(const double game_time, const double frame_time);
  protected:
    void(*click_callback)(const SDL_Event &e); //callback function
  };
};
#endif //__PUSH_BUTTON_H__
