#ifndef __VORONOI_H__
#define __VORONOI_H__

#include <vector>
#include "math_utility.h"
#include "triangulation.h"

namespace Math {
  class Voronoi2D
  {
  public:
     Voronoi2D();
    ~Voronoi2D();

    void      add_point(const Float2 &p);
    Float2    get_point(const int i) const;
    int       get_num_points() const;

    void      triangulate();

    Triangulation2D *get_triangulation();
  private:
    std::vector<Float2> pc;
    Triangulation2D delaunay;
    //KDTree2D kdt;
  };
}

#endif //__VORONOI_H__
