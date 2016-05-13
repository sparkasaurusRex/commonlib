#ifndef __VORONOI_H__
#define __VORONOI_H__

#include <vector>
#include "math_utility.h"
#include "triangulation.h"
#include "triangulation_3d.h"

namespace Math {
  class Voronoi2D
  {
  public:
     Voronoi2D();
    ~Voronoi2D();

    void          reset();
    void          add_point(const Float2 &p);
    Float2        get_point(const int i) const;
    unsigned int  get_num_points() const;

    void          triangulate();

    Triangulation2D *get_triangulation();
  private:
    std::vector<Float2> pc;
    Triangulation2D delaunay;
    //KDTree2D kdt;
  };

  class Voronoi3D
  {
  public:
    Voronoi3D();
    ~Voronoi3D();

    void      reset();
    void      add_point(const Float3 &p);
    Float3    get_point(const int i) const;
    unsigned int       get_num_points() const;

    void      triangulate();
    Triangulation3D *get_triangulation();
  private:
    std::vector<Float3> pc;
    Triangulation3D delaunay;
  };
}

#endif //__VORONOI_H__
