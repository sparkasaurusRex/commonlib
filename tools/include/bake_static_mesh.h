#ifndef __BAKE_STATIC_MESH_H__
#define __BAKE_STATIC_MESH_H__

#include <vector>

#include "draw_call.h"

namespace Tool
{
  class StaticMeshBaker
  {
  public:
    StaticMeshBaker();
    ~StaticMeshBaker() {}

    void read_xml(FILE *f);
    void write_bin(FILE *f);
  private:
    std::vector<DrawCall> draw_calls;

  };
}

#endif //__BAKE_STATIC_MESH_H__
