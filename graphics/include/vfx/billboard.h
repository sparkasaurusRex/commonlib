#pragma once

#include "renderable.h"
#include "material.h"
#include "texture.h"

namespace Graphics
{
  struct BillboardVertex
  {
    float x, y, z;
    //float r, g, b;
    float u0, v0;
  };

  class Billboard : public Renderable<BillboardVertex>
  {
  private:

  public:
    Billboard() {}
    ~Billboard() { deallocate_buffers(); }

    virtual void init();
    virtual void render(const double game_time);
    virtual void simulate(const double game_time, const double frame_time) {}
  };
};
