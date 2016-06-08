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
  friend class Sequence;
  protected:
    bool running;
    UI::Timer delay;
    UI::Timer duration;

    virtual void on_event_start() = 0;
    virtual void on_event_end() = 0;

  public:
    Event() { log = NULL; running = false; }
    ~Event() {}

    std::ostream *log;

    void set_delay(const float d) { delay.set(d); }
    void set_duration(const float d) { duration.set(d); }

    virtual void start()
    {
      (*log) << "Event::start()" << std::endl;
      delay.start();
      running = true;
    }

    virtual void init() = 0;
    virtual void simulate(const double game_time, const double frame_time)
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
  };

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

    void set_font(Graphics::Font *f) { font = f; }

    UI::Message *get_message_ui() { return msg; }

    virtual void start()
    {
      Event::start();
      (*log) << "Starting MessageEvent - " << msg->get_text().c_str() << std::endl;
    }

    virtual void init()
    {
      assert(font);
      msg = new UI::Message(font, false);
      msg->set_text(std::string("Message Event"));
      msg->init();
      msg->show(false);
    }
    virtual void simulate(const double game_time, const double frame_time)
    {
      (*log) << "MessageEvent::simulate() - " << msg->get_text().c_str() << std::endl;
      Event::simulate(game_time, frame_time);
    }
  };

  class Sequence
  {
  private:
    bool                 running;
    UI::Timer            sequence_timer;

    std::vector<Event *> events;            //all the events in the sequence
    std::vector<Event *> active_events;     //just the events that are currently active

    Event *tmp_current; //this really only works for a perfectly linear sequence
 
  public:
    Sequence() { tmp_current = NULL; }
    ~Sequence() {}

    std::ostream *log;

    void start()
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

    void reset() { }

    void add_event(Event *e) { events.push_back(e); }

    void init()
    {
    }

    void simulate(const double game_time, const double frame_time)
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
  };

  class Sequencer
  {
  private:
    std::vector<Sequence *> sequences;

  public:
    Sequencer() {}
    ~Sequencer() {}

    std::ostream *log;

    void add_sequence(Sequence *s) { s->log = log; sequences.push_back(s); }

    void simulate(const double game_time, const double frame_time)
    {
      //(*log) << "Sequencer::simulate()" << std::endl;
      for (int i = 0; i < (int)sequences.size(); i++)
      {
        sequences[i]->simulate(game_time, frame_time);
      }
    }
  };
};

#endif //__SEQUENCER_H__
