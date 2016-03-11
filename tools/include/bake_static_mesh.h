#ifndef __BAKE_STATIC_MESH_H__
#define __BAKE_STATIC_MESH_H__

#include <vector>
#include <mxml.h>

#include "draw_call.h"

namespace Tool
{
  class StaticMeshBaker
  {
  public:
    StaticMeshBaker();
    ~StaticMeshBaker() {}

    void bake(mxml_node_t *tree);

  private:
    std::vector<DrawCall> draw_calls;

  };
};

#endif //__BAKE_STATIC_MESH_H__
