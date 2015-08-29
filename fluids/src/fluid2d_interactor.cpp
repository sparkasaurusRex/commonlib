#include <stdlib.h>
#include "fluid2d_interactor.h"

Fluid2DInteractor::Fluid2DInteractor()
{
  pos = Float2(0.5f, 0.5f);
  //u = v = NULL;
  //dens = NULL;
  curr = prev = NULL;
}

Fluid2DInteractor::~Fluid2DInteractor() {}

void Fluid2DInteractor::set_pos(const Float2 p)
{
  pos = p;
}

Float2 Fluid2DInteractor::get_pos() const
{
  return pos;
}

void Fluid2DInteractor::set_grid_dimensions(const Float2 &d)
{
  fluid_dim = d;
}

void Fluid2DInteractor::set_fluid_channels(FluidChannels *c, FluidChannels *p)
{
  prev = p;
  curr = c;
}

/*
void Fluid2DInteractor::set_velocity_grid(float *_u, float *_v)
{
  u = _u;
  v = _v;
}

void Fluid2DInteractor::set_density_grid(Float3 *_dens)
{
  dens = _dens;
}
*/
