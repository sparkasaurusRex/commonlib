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
  (*log) << "Starting MessageEvent - " << msg->get_text().c_str() << std::endl;
}

void MessageEvent::simulate(const double game_time, const double frame_time)
{
  (*log) << "MessageEvent::simulate() - " << msg->get_text().c_str() << std::endl;
  Event::simulate(game_time, frame_time);
}