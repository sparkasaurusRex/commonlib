#ifndef __MESSAGE_BOX_H__
#define __MESSAGE_BOX_H__

#include "timer.h"
#include "rectangular_widget.h"

namespace UI
{
  class Message : public RectangularWidget
  {
  protected:
    std::string   main_text;
    //ToolBox       *buttons;
    Timer         timer;
    bool          use_timer;
  public:
    Message(Graphics::Font *f = NULL, bool has_timer = true);
    ~Message() {}

    virtual void show(const bool d);

    void set_text(std::string t) { main_text = t; }
    std::string get_text() const { return main_text; }

    virtual void process_event(const SDL_Event &e, const Math::Float2 offset = Math::Float2()) {}
    virtual void init() {}
    virtual void simulate(const double game_time, const double frame_time);
    virtual void render();
  };
};

#endif //__MESSAGE_BOX_H__
