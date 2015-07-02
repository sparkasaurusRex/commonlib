#ifndef __FLUID2D_INTERACTOR_H__
#define __FLUID2D_INTERACTOR_H__

#include "math_utility.h"

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
  void set_velocity_grid(float *_u, float *_v);
  void set_density_grid(float *_dens);
protected:

  inline int idx(const int &i, const int &j) const
	{
		return i + (fluid_dim[0] + 2) * j;
	}
  
  Float2 pos;

  Float2 fluid_dim; //dimensions of the fluid grid
  float *u, *v;     //velocity grid
  float *dens;      //density grid
};

#endif //__FLUID2D_INTERACTOR_H__
