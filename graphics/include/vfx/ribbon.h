#ifndef __RIBBON_H__
#define __RIBBON_H__

#include "curve.h"
#include "material.h"
#include "texture.h"
#include "renderable.h"

namespace Graphics
{
  struct RibbonVertex
  {
    float x, y, z;
    float nx, ny, nz;
    float r, g, b;
    float u0, v0;
  };

  class Ribbon : public Renderable<RibbonVertex>
  {
  public:
    Ribbon();
    ~Ribbon();

    virtual void init();
    virtual void render(const double game_time);
    virtual void simulate(const double game_time, const double frame_time);

    void set_num_segments(const int n) { num_segments = n; }
    int get_num_segments() const { return num_segments; }
  private:

    //These two curves multiply by each other to determine the height of each
    //ribbon segment.
    Math::Curve profile_a;
    Math::Curve profile_b;

    int num_segments;

    //we should probably move this out of here to generalize ribbons
    float root_speed;
    float root_scale;

    float profile_noise_scale;
    Math::Float2 profile_noise_bounds;
    float profile_noise_speed;

    float tex_scroll_speed;

    Math::Float2 theta_bounds;
    Math::Float2 phi_bounds;
  };
};

#endif //__RIBBON_H__
