#ifndef __SEQUENCER_H__
#define __SEQUENCER_H__

#include <assert.h>
#include <vector>
#include <stdio.h>

#include "timer.h"
#include "message_box.h"
#include "font.h"
#include "event.h"
#include "widget_wrangler.h"
#include "font.h"


namespace Game
{
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

    std::ostream *console_log;

    void start();

    void reset() { }
    void add_event(Event *e) { events.push_back(e); }
    void init() {}

    void simulate(const double game_time, const double frame_time);
  };

  class Sequencer
  {
  private:
    std::vector<Sequence *> sequences;

    UI::WidgetWrangler   *ww;
    Graphics::Font       *font;

  public:
    Sequencer() { ww = NULL; font = NULL; }
    ~Sequencer() {}

    Sequence *get_sequence(const int i) { return sequences[i]; }

    void bind_ui(UI::WidgetWrangler *w, Graphics::Font *f, std::ostream *cl) { ww = w; font = f; console_log = cl; }

    void read_sequence_xml(FILE *fp);
    void write_sequence_xml(FILE *fp) { assert(false); } //not yet implemented

    std::ostream *console_log;

    void add_sequence(Sequence *s) { s->console_log = console_log; sequences.push_back(s); }
    void simulate(const double game_time, const double frame_time);
  };
};

#endif //__SEQUENCER_H__
