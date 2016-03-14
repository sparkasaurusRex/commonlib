#ifndef __TOOL_H__
#define __TOOL_H__

#include <assert.h>
#include <mxml.h>
#include "math_utility.h"

namespace Tool
{
  enum AssetType
  {
    ASSET_STATIC_MESH,
    ASSET_PARTICLE_SYSTEM,

    NUM_ASSET_TYPES
  };

  Math::Float2 mxml_read_float2(const mxml_node_t *node);
  Math::Float3 mxml_read_float3(const mxml_node_t *node);
}

#endif //__ASSET_TYPES_H__
