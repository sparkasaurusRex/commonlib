#include "sequencer.h"
#include "message_event.h"
#include "wait_condition_event.h"
#include "tool.h"

using namespace Game;
using namespace std;
using namespace Math;

void Sequence::start()
{
  (*console_log) << "Sequence::start()" << std::endl;
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
    (*console_log) << "Sequence::simulate(): timer: " << sequence_timer.time_elapsed() << std::endl;

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
      (*console_log) << "sequence finished... stopping." << std::endl;
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

Condition *Sequencer::find_condition_by_hash_id(const uint32_t hash_id)
{
  for (uint32_t i = 0; i < conditions.size(); i++)
  {
    Condition *c = conditions[i];
    if (c->get_hash_id() == hash_id)
    {
      return c;
    }
  }
  return NULL;
}

void Sequencer::read_sequence_xml(FILE *fp)
{
  (*console_log) << "Sequencer::read_xml()" << endl;

  mxml_node_t *tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
  assert(tree);

  mxml_node_t *seq_node = mxmlFindElement(tree, tree, "sequence", "version", NULL, MXML_DESCEND);
  uint32_t version = atoi(mxmlElementGetAttr(seq_node, "version"));

  if (!seq_node) { return; }

  (*console_log) << "  version " << version << endl;

  Sequence *seq = new Sequence;
  seq->console_log = console_log;
  sequences.push_back(seq);

  mxml_node_t *event_node = mxmlGetFirstChild(seq_node);
  while (event_node->next != NULL)
  {
    const char *element = mxmlGetElement(event_node);
    if (element)
    {
      (*console_log) << "  " << element << endl;
      if (!_stricmp(element, "message"))
      {
        //handle message event
        double msg_delay = 0;
        double msg_duration = 10;

        const char *buff = mxmlElementGetAttr(event_node, "delay");
        if (buff) { msg_delay = atof(buff); }
        buff = mxmlElementGetAttr(event_node, "duration");
        if (buff) { msg_duration = atof(buff); }

        (*console_log) << "    delay: " << msg_delay << endl;
        (*console_log) << "    duration: " << msg_duration << endl;

        std::string message_text = Tool::mxml_read_text(event_node);

        (*console_log) << message_text.c_str() << endl;

        MessageEvent *msg_event = new MessageEvent;
        msg_event->set_delay(msg_delay);
        msg_event->set_duration(msg_duration);
        msg_event->set_font(font);
        msg_event->console_log = console_log;
        msg_event->init();

        msg_event->get_message_ui()->translate(Float2(100.0f, 100.0f));
        msg_event->get_message_ui()->set_text(message_text);
        
        ww->add_widget(msg_event->get_message_ui());

        seq->add_event(msg_event);
      }
      else if (!_stricmp(element, "wait"))
      {
        //handle wait event
        WaitConditionEvent *wc_event = new WaitConditionEvent;

        const char *buff = mxmlElementGetAttr(event_node, "condition_name");
        if (buff)
        {
          uint32_t hash_val = Math::hash_value_from_string(buff);
          (*console_log) << "    condition name: " << buff << endl;
          (*console_log) << "    hash value: " << hash_val << endl;

          //find the condition in the condition list
          Condition *c = find_condition_by_hash_id(hash_val);
          assert(c);

          wc_event->set_condition(c);
        }
        wc_event->init();
        seq->add_event(wc_event);
      }
    }
    event_node = mxmlGetNextSibling(event_node); 
  }

  seq->init();
}

void Sequencer::simulate(const double game_time, const double frame_time)
{
  //(*log) << "Sequencer::simulate()" << std::endl;
  for (int i = 0; i < (int)sequences.size(); i++)
  {
    sequences[i]->simulate(game_time, frame_time);
  }
}