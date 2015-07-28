#ifndef __TRIANGULATION_H__
#define __TRIANGULATION_H__

#include <vector>
#include "math_utility.h"
#include "shapes.h"

#define USE_CGAL 0

namespace Math {
  class Triangulation2D
  {
  public:
    Triangulation2D();
    ~Triangulation2D();

    void set_vertices(std::vector<Float2> *verts);
    void generate_delaunay_triangulation();

    std::vector<Triangle2D> *get_triangles();
  private:

    //triangulation using the CGAL library (http://www.cgal.org/)
#if USE_CGAL
    void delaunay_cgal();
#endif //USE_CGAL

    void delaunay_fortune(); //fortune's line sweep / parabolic arc algorithm

    void delaunay_merge(int start_a, int end_a, int start_b, int end_b);
    void delaunay_divide_and_conquer(int axis, int start_idx, int end_idx);

    Triangle2D create_super_triangle();
    void delaunay_super_triangle_method();

    std::vector<Float2>     *vertices;
    std::vector<Triangle2D> triangles;
    //std::vector<Edge2D>     edges;
  };
};

#endif //__TRIANGULATION_H__
