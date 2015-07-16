#include "fluid2d_turbulence.h"
#include "perlin.h"

Fluid2DTurbulenceField::Fluid2DTurbulenceField() : Fluid2DInteractor()
{
  octaves = 1;
  scale = 1.0f;
  strength = 1.0f;
  speed = 1.0f;

  time = 0;
}

Fluid2DTurbulenceField::~Fluid2DTurbulenceField() {}

void Fluid2DTurbulenceField::simulate(const float dt)
{
  time += dt;
  for(int i = 0; i < fluid_dim[0]; i++)
  {
    for(int j = 0; j < fluid_dim[1]; j++)
    {
      float x = (float)i / (float)fluid_dim[0];
      float y = (float)j / (float)fluid_dim[1];
      u[idx(i, j)] += strength * PerlinNoise::scaled_octave_noise_3d(octaves, 1.0f, scale, -1.0f, 1.0f, x + phase[0], y + phase[1], speed * time);
    }
  }
}
