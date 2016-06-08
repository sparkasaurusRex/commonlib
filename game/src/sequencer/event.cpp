#include "event.h"

using namespace Game;

void Event::simulate(const double game_time, const double frame_time)
{
  if (!running) { return; }

  if (delay.is_running())
  {
    if (delay.has_elapsed())
    {
      delay.stop();
      duration.start();
      on_event_start();
    }
    (*log) << "    Delayed: " << delay.time_remaining() << std::endl;
  }

  if (duration.is_running())
  {
    if (duration.has_elapsed())
    {
      duration.stop();
      running = false;
      on_event_end();
    }
    (*log) << "    Running: " << duration.time_remaining() << std::endl;
  }
}