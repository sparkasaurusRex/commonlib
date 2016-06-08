#pragma once

#include <assert.h>
#include <ostream>
#include "font.h"
#include "timer.h"
#include "message_box.h"

namespace Game
{
  class Event
  {
    friend class Sequence;
  protected:
    bool running;
    UI::Timer delay;
    UI::Timer duration;

    virtual void on_event_start() = 0;
    virtual void on_event_end() = 0;

  public:
    Event() { console_log = NULL; running = false; }
    ~Event() {}

    std::ostream *console_log;

    void set_delay(const float d) { delay.set(d); }
    void set_duration(const float d) { duration.set(d); }

    virtual void start();

    virtual void init() = 0;
    virtual void simulate(const double game_time, const double frame_time);
  };
};
