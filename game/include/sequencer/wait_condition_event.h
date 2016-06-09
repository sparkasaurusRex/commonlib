#pragma once

#include "event.h"
#include "condition.h"

namespace Game
{
  class WaitConditionEvent : public Event
  {
  private:
    Condition *condition;

    virtual void on_event_start() {}
    virtual void on_event_end() {}
  public:
    WaitConditionEvent() {}
    ~WaitConditionEvent() {}

    void set_condition(Condition *c) { condition = c; }

    virtual void init() {}
    virtual void start() { running = true; }
    virtual void simulate(const double game_time, const double frame_time)
    {
      if (condition)
      {
        running = !condition->condition_met();
      }
      else
      {
        running = false;
      }
    }
  };
};