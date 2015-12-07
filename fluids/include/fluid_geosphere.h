#ifndef __FLUID_GEOSPHERE_H__
#define __FLUID_GEOSPHERE_H__

#include "geodesic_grid.h"
#include "fluid_channels.h"

class FluidGeodesicSphere
{
public:
  FluidGeodesicSphere();
  ~FluidGeodesicSphere();

  void simulate(const float dt);
private:
  int num_subdivisions;
  GeodesicGrid<FluidChannels> prev, curr;
};

#endif //__FLUID_GEOSPHERE_H__
