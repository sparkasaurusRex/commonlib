#ifndef __BAKERY_H__
#define __BAKERY_H__

#include <string>

#include "static_mesh_baker.h"
#include "bphys_baker.h"
#include "shader_baker.h"

namespace Tool
{
  class Bakery
  {
  public:
    Bakery() {}
    ~Bakery() {}

    void init();

    void bake(std::string fname, std::string out_fname);
  private:
    StaticMeshBaker   static_mesh_baker;
    BPhysBaker        bphys_baker;
    ShaderBaker       shader_baker;
  };
};

#endif //__BAKERY_H__
