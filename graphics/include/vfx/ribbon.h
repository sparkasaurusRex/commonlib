#ifndef __RIBBON_H__
#define __RIBBON_H__

#include "curve.h"
#include "material.h"
#include "texture.h"

namespace Graphics
{
  class Ribbon
  {
  public:
    Ribbon();
    ~Ribbon();

    void init();
    void render();
    void simulate(const float game_time, const float frame_time);

    void set_num_segments(const int n) { num_segments = n; }
    int get_num_segments() const { return num_segments; }
  private:

    //These two curves multiply by each other to determine the height of each
    //ribbon segment.
    Math::Curve profile_a;
    Math::Curve profile_b;

    Material mat;
    Texture tex_a, tex_b;

    GLuint vbo, ibo;

    int num_segments;

    struct RibbonVertex
    {
      float x, y, z;
      float nx, ny, nz;
      float r, g, b;
      float u0, v0;
    };

    int num_verts;
    RibbonVertex *vertex_data;

    int num_indices;
    unsigned int *index_data;

    //we should probably move this out of here to generalize ribbons
    float root_speed;
    float root_scale;

    float tex_scroll_speed;
  };
};

#endif //__RIBBON_H__
