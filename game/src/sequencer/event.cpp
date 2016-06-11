#include "event.h"

using namespace Game;

void Event::start()
{
  (*console_log) << "Event::start()" << std::endl;
  delay.start();
  running = true;
}

void Event::halt()
{
  delay.stop();
  duration.stop();
  running = false;
  on_event_end();
}

void Event::simulate(const double game_time, const double frame_time)
{
  if (!running) { return; }

  if (delay.is_running())
  {
    if (delay.has_elapsed())
    {
      if (!condition)
      {
        duration.start();
      }
      delay.stop();
      on_event_start();
    }
    //(*console_log) << "    Delayed: " << delay.time_remaining() << std::endl;
  }

  if (condition && condition->condition_met())
  {
    halt();
  }
  else
  {
    if (duration.is_running() && duration.has_elapsed())
    {
       halt();
      //(*console_log) << "    Running: " << duration.time_remaining() << std::endl;
    }
  }
}