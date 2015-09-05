#ifndef __FADE_SCREEN_H__
#define __FADE_SCREEN_H__

#include <font.h>
#include <string>

//TODO: find a better home for this
class Timer
{
public:
  Timer() { time_left = total_time = -1.0f; active = false; }
  ~Timer() {}

  void set(const float duration) { time_left = total_time = duration; }
  void activate() { active = true; }
  void simulate(const float dt) { if(active) time_left -= dt; }

  float get_time_left() const { return time_left; }
  float pct_elapsed() const { return 1.0f - (time_left / total_time); }
  bool is_active() const { return time_left > 0.0f; }
private:
  float total_time;     //original duration the timer was set for
  float time_left;      //how time is currently left
  bool active;          //true if the timer is actively counting down
};

class FadeScreen //: UIElement
{
public:
  FadeScreen();
  ~FadeScreen() {}

  void set_font(Font *f);
  void simulate(const float dt);

  void render_gl() const;

  void play();
  bool is_active() const;

  void set_text(std::string t) { text = t; }
private:
  Font *font;

  Timer fade_in_timer;
  Timer linger_timer;     //do you have to let it?
  Timer fade_out_timer;

  float fade_opacity;
  std::string text;
};

#endif //__FADE_SCREEN_H__
