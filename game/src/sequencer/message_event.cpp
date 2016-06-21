#include <assert.h>
#include "message_event.h"

using namespace Game;

void MessageEvent::init()
{
  assert(font);
  msg = new UI::Message(font, false);
  msg->set_text(std::string("Message Event"));
  msg->init();
  msg->show(false);
}

void MessageEvent::start()
{
  Event::start();
  //(*console_log) << "Starting MessageEvent - " << msg->get_text().c_str() << std::endl;
}

void MessageEvent::simulate(const double game_time, const double frame_time)
{
  //(*console_log) << "MessageEvent::simulate() - " << msg->get_text().c_str() << std::endl;
  Event::simulate(game_time, frame_time);
}

void MessageEvent::on_event_start()
{
  //(*console_log) << "MessageEvent::on_event_start()" << std::endl;
  msg->show(true);
}

void MessageEvent::on_event_end()
{
  msg->show(false);
}