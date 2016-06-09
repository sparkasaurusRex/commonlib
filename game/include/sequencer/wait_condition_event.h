#pragma once

#include "event.h"

namespace Game
{
  class Condition
  {
  protected:
  public:
    Condition() {}
    ~Condition() {}

    virtual bool condition_met() const = 0;
  };

  class ConditionBool : public Condition
  {
  protected:
    bool *var;
  public:
    ConditionBool() : Condition() {}
    ~ConditionBool() {}

    virtual bool condition_met() { return *var; }

    void set_var_ptr(bool *b) { var = b; }
  };

  class WaitConditionEvent : public Event
  {
  private:
    Condition *c;

    virtual void on_event_start() {}
    virtual void on_event_end() {}
  public:
    WaitConditionEvent() {}
    ~WaitConditionEvent() {}

    virtual void init() {}
    virtual void start() {}
    virtual void simulate(const double game_time, const double frame_time) {}
  };
};