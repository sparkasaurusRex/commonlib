#ifndef __FLUID2D_INFLOW_H__
#define __FLUID2D_INFLOW_H__

#include "fluid2d_interactor.h"

class Fluid2DInflow : public Fluid2DInteractor
{
public:
  Fluid2DInflow();
  ~Fluid2DInflow();

  void simulate(const float dt);
protected:
  float radius;
  float rate;
  Float2 velocity;
};

#endif //__FLUID2D_INFLOW_H__
