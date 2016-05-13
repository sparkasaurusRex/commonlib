#ifndef __AABB_H__
#define __AABB_H__

#include "math_utility.h"

namespace Math {

  class AABB2D {
  public:
    AABB2D(const Float2 pt);
    ~AABB2D() {}

    Float2 dim() const;
    void grow(const Float2 p);
    int get_min_axis() const;
    int get_max_axis() const;

  private:
    Float2 min_pt;
    Float2 max_pt;
  };

  class AABB3D {
  public:
    AABB3D(const Float3 pt);
    ~AABB3D() {}

    Float3 dim() const;
    void grow(const Float3 p);
    int get_min_axis() const;
    int get_max_axis() const;

  private:
    Float3 min_pt;
    Float3 max_pt;
  };
}

#endif //__AABB_H__