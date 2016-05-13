#ifndef __FLUID2D_INTERACTOR_H__
#define __FLUID2D_INTERACTOR_H__

#include "math_utility.h"
#include "fluid_channels.h"

using namespace Math;

class Fluid2DInteractor
{
public:
  Fluid2DInteractor();
  ~Fluid2DInteractor();

  virtual void simulate(const float dt) = 0;

  void set_pos(const Float2 p);
  Float2 get_pos() const;

  void set_grid_dimensions(const Float2 &d);
  //void set_velocity_grid(float *_u, float *_v);
  //void set_density_grid(Float3 *_dens);
  void set_fluid_channels(FluidChannels *c, FluidChannels *p);
protected:

  inline int idx(const int &i, const int &j) const
	{
		return i + ((int)fluid_dim[0] + 2) * j;
	}

  Float2 pos;

  Float2 fluid_dim;   //dimensions of the fluid grid
  //float *u, *v;       //velocity grid
  //Float3 *dens;       //density grid
  FluidChannels *prev, *curr;
};

#endif //__FLUID2D_INTERACTOR_H__
