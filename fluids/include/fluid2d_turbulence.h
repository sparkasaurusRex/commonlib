#ifndef __FLUID2D_TURBULENCE_H__
#define __FLUID2D_TURBULENCE_H__

#include "fluid2d_interactor.h"

class Fluid2DTurbulenceField : public Fluid2DInteractor
{
  public:
    Fluid2DTurbulenceField();
    ~Fluid2DTurbulenceField();

    void simulate(const float dt);

    void set_octaves(const int o) { octaves = o; }
    void set_scale(const float s) { scale = s; }
    void set_speed(const float s) { speed = s; }
    void set_strength(const float s) { strength = s; }
    void set_phase(const Float2 &p) { phase = p; }

  protected:
    int octaves;
    float scale;
    float speed;
    float strength;
    Float2 phase;

    float time;
};

#endif //__FLUID2D_TURBULENCE_H__
