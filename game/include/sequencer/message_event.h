#pragma once

#include "event.h"

namespace Game
{
  class MessageEvent : public Event
  {
  private:
    UI::Message     *msg;
    Graphics::Font  *font;

    virtual void on_event_start()
    {
      (*log) << "MessageEvent::on_event_start()" << std::endl;
      msg->show(true);
    }

    virtual void on_event_end()
    {
      msg->show(false);
    }

  public:
    MessageEvent() : Event() { msg = NULL; font = NULL; }
    ~MessageEvent() { if (msg) { delete msg; } }

    virtual void init();
    virtual void start();
    virtual void simulate(const double game_time, const double frame_time);

    void set_font(Graphics::Font *f) { font = f; }

    UI::Message *get_message_ui() { return msg; }
  };
};