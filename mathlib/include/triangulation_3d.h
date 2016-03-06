#ifndef __TRIANGULATION_3D_H__
#define __TRIANGULATION_3D_H__

//
// Methods to triangulate a point cloud of sites in 3D space
// TODO: NOT EVEN CLOSE TO BEING IMPLEMENTED!!!
//

#include <vector>
#include <queue>
#include "math_utility.h"
#include "shapes.h"

namespace Math
{
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
    std::vector<Edge3D>   *get_edges();
  private:
    std::vector<Float3>   *vertices;
    std::vector<Edge3D>   edges;
  };
};

#endif //__TRIANGULATION_3D_H__
