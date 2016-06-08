#include "sequencer.h"

using namespace Game;
using namespace std;

void Sequence::start()
{
  (*log) << "Sequence::start()" << std::endl;
  if (events.size() > 0)
  {
    running = true;
    sequence_timer.start();

    tmp_current = events[0];
    tmp_current->start();
  }
}

void Sequence::simulate(const double game_time, const double frame_time)
{
  if (running)
  {
    (*log) << "Sequence::simulate(): timer: " << sequence_timer.time_elapsed() << std::endl;

    if (tmp_current)
    {
      tmp_current->simulate(game_time, frame_time);
      if (!tmp_current->running)
      {
        std::vector<Event *>::iterator curr = std::find(events.begin(), events.end(), tmp_current);
        assert(curr != events.end()); //this would be very strange, not to find the event in the list
        std::vector<Event *>::iterator next_event = curr + 1;
        if (next_event == events.end())
        {
          running = false; //we're done!
          sequence_timer.stop();
        }
        else
        {
          tmp_current = (*next_event);
          tmp_current->start();
        }
      }
    }
    else
    {
      (*log) << "sequence finished... stopping." << std::endl;
      running = false;
      sequence_timer.stop();
    }

    //see which events should be active
    /*active_events.clear();
    for (int i = 0; i < (int)events.size(); i++)
    {

    }*/
  }
}


void Sequencer::simulate(const double game_time, const double frame_time)
{
  //(*log) << "Sequencer::simulate()" << std::endl;
  for (int i = 0; i < (int)sequences.size(); i++)
  {
    sequences[i]->simulate(game_time, frame_time);
  }
}