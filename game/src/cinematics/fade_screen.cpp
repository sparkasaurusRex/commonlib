#include <iostream>
#include "fade_screen.h"
#include "math_utility.h"

using namespace std;
using namespace Math;

FadeScreen::FadeScreen()
{
  fade_in_timer.set(2.0f);
  linger_timer.set(2.0f);
  fade_out_timer.set(1.0f);

  font = NULL;

  fade_opacity = 0.0f;
}

void FadeScreen::set_font(Font *f)
{
  font = f;
}

void FadeScreen::simulate(const float dt)
{
  if(fade_in_timer.is_active())
  {
    cout<<"fade in"<<endl;
    fade_in_timer.simulate(dt);
    fade_opacity = fade_in_timer.pct_elapsed();
    if(fade_opacity >= 1.0f)
    {
      fade_opacity = 1.0f;
      linger_timer.activate();
    }
  }
  else if(linger_timer.is_active())
  {
    //cout<<"linger"<<endl;
    linger_timer.simulate(dt);
    fade_opacity = 1.0f;
    if(linger_timer.pct_elapsed() >= 1.0f)
    {
      fade_out_timer.activate();
    }
  }
  else if(fade_out_timer.is_active())
  {
    //cout<<"fade out"<<endl;
    fade_out_timer.simulate(dt);
    fade_opacity = clamp(1.0f - fade_out_timer.pct_elapsed(), 0.0f, 1.0f);
  }
  //cout<<"title screen opacity: "<<fade_opacity<<endl;
}

void FadeScreen::render_gl() const
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor4f(1.0f, 1.0f, 1.0f, fade_opacity);

  font->print(10, 10, text.c_str());
}

void FadeScreen::play()
{
  fade_in_timer.activate();
}

bool FadeScreen::is_active() const
{
  if(fade_in_timer.is_active()) return true;
  if(linger_timer.is_active()) return true;
  if(fade_out_timer.is_active()) return true;
  return false;
}
