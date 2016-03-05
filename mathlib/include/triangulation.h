#ifndef __TRIANGULATION_H__
#define __TRIANGULATION_H__

#include <vector>
#include <queue>
#include "math_utility.h"
#include "topology.h"
#include "shapes.h"

//#define USE_CGAL 0
//#define USE_BOOST

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

  class Triangulation3D
  {
  public:
    Triangulation3D();
    ~Triangulation3D();

    void reset() {}
    void set_vertices(std::vector<Float3> *verts);

    void generate_delaunay_triangulation();
    void generate_convex_hull();

    //std::vector<Triangle2D> *get_triangles();
    std::vector<Edge3D> *get_edges();
  private:
    std::vector<Float3>   *vertices;
    std::vector<Edge3D>   edges;
  };

  class TriangulationSphere
  {
  public:
    TriangulationSphere();
    ~TriangulationSphere();

    void reset() {}
    void set_vertices(std::vector<Float3> *verts);

    void generate_delaunay_triangulation();

    void fortune_step();

    float get_beach_line_height() { return beach_line_height; }
    void advance_beach_line(const float dh);

    std::vector<Edge3D> *get_edges() { return &edges; }
  private:

    void handle_site_event(Float3 *p);

    std::vector<Float3>     *vertices;
    std::vector<Edge2D>     edges;

    //fortune algorithm variables
    float                   beach_line_height;

    class SiteCompare
    {
    public:
      bool operator()(std::pair<Float3, int> a, std::pair<Float3, int> b)
      {
        if(a.first._val[1] < b.first._val[1]) { return true; }
        if(a.first._val[1] > b.first._val[1]) { return false; }

        //y coords are equal, so sort based on longitude
        float long_a = 0.5f + (atan2(a.first._val[2], a.first._val[0]) / M_PI) * 0.5f;
        float long_b = 0.5f + (atan2(b.first._val[2], b.first._val[0]) / M_PI) * 0.5f;
        return (long_a < long_b);
      }
    };

    std::priority_queue<Float3> circle_events;
    std::priority_queue<std::pair<Float3, int>, std::vector<std::pair<Float3, int> >, SiteCompare> site_events;

    //use skip list / binary tree to store beach line
    //each node represents an arc (beta) on the beach line
  };
};

#endif //__TRIANGULATION_H__
