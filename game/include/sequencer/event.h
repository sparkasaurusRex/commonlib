#pragma once

#include <ostream>
#include "font.h"
#include "timer.h"
#include "condition.h"

namespace Game
{
  enum BooleanOperator
  {
    BOOLEAN_A,
    BOOLEAN_B,
    BOOLEAN_AND,
    BOOLEAN_OR,
    BOOLEAN_XOR,
    BOOLEAN_NAND,
    BOOLEAN_NOT,
    NUM_BOOLEAN_OPERATORS
  };

  class Event
  {
    friend class Sequence;
  protected:
    bool              running;
    UI::Timer         delay;

    Condition         *condition;
    UI::Timer         duration;
    BooleanOperator   combine_operator;

    virtual void on_event_start() = 0;
    virtual void on_event_end() = 0;

    void halt();

  public:
    Event() { console_log = NULL; running = false; combine_operator = BOOLEAN_A; }
    ~Event() {}

    std::ostream *console_log;

    void set_delay(const double d)    { delay.set(d); }
    void set_duration(const double d) { duration.set(d); }
    void set_condition(Condition *c) { condition = c; }
    void set_combine_operator(BooleanOperator bo) { combine_operator = bo; }

    virtual void start();

    virtual void init() = 0;
    virtual void simulate(const double game_time, const double frame_time);
  };
};
