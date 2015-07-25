#ifndef __TRIANGULATION_H__
#define __TRIANGULATION_H__

#include <vector>
#include "math_utility.h"

namespace Math {

  class Triangle2D
  {
    public:
      Triangle2D() { indices[0] = indices[1] = indices[2] = 0; }
      ~Triangle2D() {}

      //operator=()

      int indices[3];
  };

  /*class Edge2D
  {
    public:
      Edge2D() { indices[0] = indices[1] = 0; }
      ~Edge2D() {}
      int indices[2];
  };*/

  class Triangulation2D
  {
  public:
    Triangulation2D();
    ~Triangulation2D();

    void set_vertices(std::vector<Float2> *verts);
    void generate_delaunay_triangulation();

    //int get_num_triangles() const;
    //Triangle2D *get_triangle(int idx) const;
    std::vector<Triangle2D> *get_triangles();
  private:
    std::vector<Float2>     *vertices;
    std::vector<Triangle2D> triangles;
    //std::vector<Edge2D>     edges;
  };
};

#endif //__TRIANGULATION_H__
