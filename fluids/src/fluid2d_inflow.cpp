#include <iostream>
#include "fluid2d_inflow.h"

using namespace std;

Fluid2DInflow::Fluid2DInflow() : Fluid2DInteractor()
{
  rate = 200.0f;
  velocity = Float2(10.0f, 0.0f);
  radius = 0.025f;
}

Fluid2DInflow::~Fluid2DInflow() {}

void Fluid2DInflow::simulate(const float dt)
{
  //cout<<fluid_dim[0]<<", "<<fluid_dim[1]<<endl;
  for(int i = 0; i < fluid_dim[0]; i++)
  {
    for(int j = 0; j < fluid_dim[1]; j++)
    {
      Float2 p0((float)i / (float)fluid_dim[0], (float)j / (float)fluid_dim[1]);
      Float2 dir(pos[0] - p0[0], pos[1] - p0[1]);
      float d2 = dir.mag_squared();

      float r2 = radius * radius;

      if(d2 < r2)
      {
        u[idx(i, j)] += velocity[0];
        v[idx(i, j)] += velocity[1];

        dens[idx(i, j)] += rate * dt * (1.0f - d2 / (radius * radius));
      }
    }
  }
}
