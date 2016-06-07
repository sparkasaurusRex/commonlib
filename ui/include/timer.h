#ifndef __TIMER_H__
#define __TIMER_H__

#if defined (_WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif //_WIN32

namespace UI
{
  class Timer
  {
  private:
    Uint32 start_time;
    double duration;
    bool running;
  public:
    Timer() { start_time = 0; duration = 1.0; running = false; }
    ~Timer() {}

    bool is_running() const { return running; }
    bool has_elapsed() const { return time_remaining() <= 0.0; }

    void start() { start_time = SDL_GetTicks(); running = true; }
    void stop() { running = false; }
    void set(const double t) { duration = t; }
    double time_elapsed() const { Uint32 curr_time = SDL_GetTicks(); return (double)(curr_time - start_time) / 1000.0; }
    double time_remaining() const { Uint32 curr_time = SDL_GetTicks(); return duration - (double)(curr_time - start_time) / 1000.0; }
    float pct_elapsed() const { return (float)(time_remaining() / duration); }

    void simulate(const double game_time, const double frame_time) {}

    //TODO: callback function when timer is elapsed
  };
};

#endif //__TIMER_H__