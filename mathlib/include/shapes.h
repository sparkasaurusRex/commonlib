#ifndef __SHAPES_H__
#define __SHAPES_H__

#include <vector>
#include "math_utility.h"

namespace Math {
  class Edge2D
  {
  public:
    Edge2D() { vidx[0] = vidx[1] = 0; }
    ~Edge2D() {}

    int vidx[2]; //indices of points
  };

  typedef Edge2D Edge3D;

  class Circle2D
  {
  public:
    Circle2D() { r = 1.0f; }
    ~Circle2D() {}

    float r;
    Float2 center;
  };

  class Triangle2D
  {
    public:
      Triangle2D() { vidx[0] = vidx[1] = vidx[2] = 0; }
      ~Triangle2D() {}

      //operator=()
      Circle2D circumcircle(std::vector<Float2> *vertices) const;

      int vidx[3];
      //std::vector<Float2> *vertices;
  };
};

#endif //__SHAPES_H__
