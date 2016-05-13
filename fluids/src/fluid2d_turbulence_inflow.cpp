#include "fluid2d_turbulence_inflow.h"
#include "perlin.h"

Fluid2DTurbulenceInflow::Fluid2DTurbulenceInflow()
{
  octaves = 1;
  scale = 1.0f;
  strength = 1.0f;
  speed = 1.0f;

  time = 0;
}

Fluid2DTurbulenceInflow::~Fluid2DTurbulenceInflow()
{

}

void Fluid2DTurbulenceInflow::simulate(const float dt)
{
  time += dt;
  for(int i = 0; i < fluid_dim[0]; i++)
  {
    for(int j = 0; j < fluid_dim[1]; j++)
    {
      float x = (float)i / (float)fluid_dim[0];
      float y = (float)j / (float)fluid_dim[1];

      float in_flow = strength * PerlinNoise::scaled_octave_noise_3d((float)octaves, 1.0f, scale, 0.0f, 1.0f, x + phase[0], y + phase[1], speed * time);
      curr[idx(i, j)].data[channel] += in_flow;
    }
  }
}
