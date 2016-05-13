#ifndef __VORONOI_SPHERE_H__
#define __VORONOI_SPHERE_H__

#include "math_utility.h"
#include "triangulation_sphere.h"

namespace Math
{
  class VoronoiSphere
  {
  public:
    VoronoiSphere();
    ~VoronoiSphere();

    void          add_point(const Float3 &p);
    Float3        get_point(const int i) const;

    unsigned int  get_num_points() const { return (unsigned int)pc.size(); }

    void          triangulate();

    void          triangulation_step(const float dh);

    TriangulationSphere *get_triangulation();
  protected:
    std::vector<Float3> pc;
    TriangulationSphere delaunay;
  };
};

#endif //__VORONOI_SPHERE_H__
