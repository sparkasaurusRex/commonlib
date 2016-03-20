#ifndef __BAKERY_H__
#define __BAKERY_H__

#include <string>

#include "static_mesh_baker.h"
#include "bphys_baker.h"

namespace Tool
{
  class Bakery
  {
  public:
    Bakery() {}
    ~Bakery() {}

    void init();

    void bake(std::string fname);
  private:
    StaticMeshBaker   smb;
    BPhysBaker        bpb;
  };
};

#endif //__BAKERY_H__
