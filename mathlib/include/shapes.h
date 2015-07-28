#ifndef __SHAPES_H__
#define __SHAPES_H__

#include <vector>

#include "math_utility.h"


/*class Edge2D
{
  public:
    Edge2D() { indices[0] = indices[1] = 0; }
    ~Edge2D() {}
    int indices[2];
};*/

namespace Math {
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
      Triangle2D() { indices[0] = indices[1] = indices[2] = 0; }
      ~Triangle2D() {}

      //operator=()
      Circle2D circumcircle() const;

      int indices[3];
      std::vector<Float2> *vertices;
  };
};

#endif //__SHAPES_H__
