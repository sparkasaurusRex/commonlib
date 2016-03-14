#ifndef __BAKERY_H__
#define __BAKERY_H__

#include <string>

#include "static_mesh_baker.h"

namespace Tool
{
  class Bakery
  {
  public:
    Bakery() {}
    ~Bakery() {}

    void bake(std::string fname);
  private:
    StaticMeshBaker smb;
  };
};

#endif //__BAKERY_H__
