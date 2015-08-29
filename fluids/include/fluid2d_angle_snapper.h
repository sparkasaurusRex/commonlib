#ifndef __FLUID2D_ANGLE_SNAPPER__
#define __FLUID2D_ANGLE_SNAPPER__

#include "fluid2d_interactor.h"

class Fluid2DAngleSnapper : public Fluid2DInteractor
{
  public:
    Fluid2DAngleSnapper();
    Fluid2DAngleSnapper(const int _num_angles);
    ~Fluid2DAngleSnapper();

    void set_strength(const float s) { strength = s; }
    void set_channel(FluidChannelType c) { channel = c; }

    void simulate(const float dt);

  protected:
    void allocate_angles();

    int num_angles;
    Float2 *angles;

    float strength;

    FluidChannelType channel;
};

#endif
