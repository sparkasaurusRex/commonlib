#ifndef __BPHYS_BAKER_H__
#define __BPHYS_BAKER_H__

#include <stdio.h>
//class to bake data from a blender physics cache (fluid sim, particles, etc)
//to textures or vertex colors usable for realtime applications

namespace Tool
{
  enum BPhysDataType
  {
    BPHYS_DATA_SOFT_BODY,
    BPHYS_DATA_PARTICLES,
    BPHYS_DATA_CLOTH,
    BPHYS_DATA_SMOKE_LOW,
    BPHYS_DATA_SMOKE_HIGH,
    BPHYS_DATA_DYNAMICPAINT,
    BPHYS_DATA_RIGID_BODY
  };

  struct BPhysHeader
  {
    char bphys[8];
    char data_type;
  };

  class BPhysBaker
  {
  public:
    BPhysBaker() {}
    ~BPhysBaker() {}

    void init();

    void bake(FILE *f);
  private:
    void read_smoke_data(FILE *f);
  };
};

#endif //__BPHYS_BAKER_H__
