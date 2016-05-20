#ifndef __METER_H__
#define __METER_H__

#include "rectangular_widget.h"

namespace UI
{
  class Meter : public RectangularWidget
  {
  public:
    Meter(Font *f = NULL);
    ~Meter() {}

    virtual void process_event(const SDL_Event &e);

    virtual void init() {}
    virtual void simulate(const float dt) {}
    virtual void render();

    void set_percent(const float p) { pct = p; }
    float get_percent() const { return pct;}
  private:
    float pct; //0 - 1
  };
};

#endif //__METER_H__
