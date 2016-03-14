#ifndef __STATIC_MESH_BAKER_H__
#define __STATIC_MESH_BAKER_H__

#include <vector>

#include "tool.h"
#include "draw_call.h"
#include "static_mesh.h"

namespace Tool
{
  class MeshFace
  {
  public:
    Float3 normal;
    Float2 uvs[2];

    int mat_idx;
  };

  class StaticMeshBaker
  {
  public:
    StaticMeshBaker();
    ~StaticMeshBaker() {}

    void bake(mxml_node_t *tree);

  private:
    std::vector<DrawCall<Graphics::StaticMeshVertex> > draw_calls;
  };
};

#endif //__STATIC_MESH_BAKER_H__
