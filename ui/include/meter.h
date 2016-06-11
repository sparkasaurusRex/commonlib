#ifndef __METER_H__
#define __METER_H__

#include "math_utility.h"
#include "rectangular_widget.h"

namespace UI
{
  class Meter : public RectangularWidget
  {
  public:
    Meter(Graphics::Font *f = NULL);
    ~Meter() {}

    virtual void process_event(const SDL_Event &e, const Math::Float2 offset = Math::Float2());

    virtual void init() {}
    virtual void simulate(const double game_time, const double frame_time) { RectangularWidget::simulate(game_time, frame_time); }
    virtual void render();

    void set_percent(const float p) { pct = p; }
    float get_percent() const { return pct;}
  private:
    float pct; //0 - 1
  };
};

#endif //__METER_H__
