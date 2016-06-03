#ifndef __SEQUENCER_H__
#define __SEQUENCER_H__

#include "timer.h"
#include "message_box.h"
#include "font.h"

namespace Game
{
  /*
  enum SequencerNodeTypes
  {
    SEQUENCER_NODE_LOOPING,
    SEQUENCER_NODE_SEQUENTIAL,

    NUM_SEQUENCER_NODE_TYPES
  };

  class SequencerNode
  {
  public:
    SequencerNode() {}
    ~SequencerNode() {}
  };
  */

  class Event
  {
  private:
    bool running;
    UI::Timer delay;
    UI::Timer duration;
  public:
    Event() {}
    ~Event() {}

    void set_delay(const float d) { delay.set(d); }

    virtual void start() { delay.start(); }

    virtual void init() = 0;
    virtual void simulate(const double game_time, const double frame_time)
    {
      if (running)
      {
        delay.simulate(game_time, frame_time);
      }
    }
  };

  class MessageEvent : public Event
  {
  private:
    UI::Message     *msg;
    Graphics::Font  *font;
  public:
    MessageEvent() : Event() { msg = NULL; font = NULL; }
    ~MessageEvent() { if (msg) { delete msg; } }

    void set_font(Graphics::Font *f) { font = f; }

    virtual void init() { assert(font); msg = new UI::Message(font); msg->set_text(std::string("Message Event")); }
    virtual void simulate(const double game_time, const double frame_time)
    {
      Event::simulate(game_time, frame_time);
    }
  };

  class Sequence
  {
  private:
    bool      running;
    UI::Timer sequence_timer;

    std::vector<Event *> events;            //all the events in the sequence
    std::vector<Event *> active_events;     //just the events that are currently active

    Event *tmp_current; //this really only works for a perfectly linear sequence
 
  public:
    Sequence() {}
    ~Sequence() {}

    void start() { if (events.size() > 0) { running = true; tmp_current = events[0]; } }
    void reset() { }

    void add_event(Event *e) { events.push_back(e); }

    void init() {}
    void simulate(const double game_time, const double frame_time)
    {
      if (running)
      {
        //see which events should be active
        active_events.clear();
        for (int i = 0; i < (int)events.size(); i++)
        {
          
        }
      }
    }
  };

  class Sequencer
  {
  private:
    std::vector<Sequence *> sequences;
  public:
    Sequencer() {}
    ~Sequencer() {}

    void add_sequence(Sequence *s) { sequences.push_back(s); }

    void simulate(const double game_time, const double frame_time)
    {
      for (int i = 0; i < (int)sequences.size(); i++)
      {
        sequences[i]->simulate(game_time, frame_time);
      }
    }
  };
};

#endif //__SEQUENCER_H__
