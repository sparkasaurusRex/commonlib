#ifndef __TRIANGULATION_H__
#define __TRIANGULATION_H__

#include <vector>
#include <queue>
#include "math_utility.h"
#include "topology.h"
#include "shapes.h"
#include "voronoi_sphere_ellis.h"

namespace Math {
  class Triangulation2D
  {
  public:
    Triangulation2D();
    ~Triangulation2D();

    void reset();

    void set_vertices(std::vector<Float2> *verts);
    void generate_delaunay_triangulation();

    void generate_convex_hull();

    std::vector<Triangle2D> *get_triangles();
    std::vector<Edge2D> *get_edges();
  private:

    VoronoiSphere voronoi;

    //convex hull algorithms
    void quick_hull();
    void graham_scan_convex_hull();

    //triangulation using the CGAL library (http://www.cgal.org/)
#if defined(__USE_CGAL__)
    void delaunay_cgal();
    void delaunay_cgal_sphere();
#endif //USE_CGAL

#if defined(__USE_BOOST__)
    void delaunay_boost();
#endif

    void delaunay_fortune(); //fortune's line sweep / parabolic arc algorithm

    void delaunay_merge(int start_a, int end_a, int start_b, int end_b);
    void delaunay_divide_and_conquer(int axis, int start_idx, int end_idx);

    Triangle2D create_super_triangle();
    void delaunay_super_triangle_method();

    std::vector<Float2>     *vertices;
    std::vector<Triangle2D> triangles;
    std::vector<Edge2D>     edges;
  };
};

#endif //__TRIANGULATION_H__
