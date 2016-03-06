#include <assert.h>
#include <iostream>
#include <list>
#include "triangulation_3d.h"

#if defined(__USE_CGAL3D__)
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/algorithm.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/convex_hull_3_to_polyhedron_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Polyhedron_3<K>                     Polyhedron_3;
typedef K::Segment_3                              Segment_3;
typedef K::Point_3                                Point_3;
typedef CGAL::Creator_uniform_3<double, Point_3>  PointCreator;
typedef CGAL::Delaunay_triangulation_3<K>         Delaunay;

#endif //__USE_CGAL3D__

using namespace Math;
using namespace std;

Triangulation3D::Triangulation3D()
{
  vertices = NULL;
}

Triangulation3D::~Triangulation3D()
{}

void Triangulation3D::set_vertices(std::vector<Float3> *verts)
{
  vertices = verts;
}

void Triangulation3D::generate_delaunay_triangulation()
{
  generate_convex_hull();
}

void Triangulation3D::generate_convex_hull()
{
#if defined(__USE_CGAL3D__)

  std::list<Point_3> points;
  for(int i = 0; i < vertices->size(); i++)
  {
    Float3 v = (*vertices)[i];
    Point_3 p(v[0], v[1], v[2]);
    points.push_back(p);
  }

  Delaunay dt;
  dt.insert(points.begin(), points.end());

  //get only the surface
  /*CGAL::Polyhedron_3<K> poly;
  convex_hull_3_to_polyhedron_3(dt, poly);

  //add all the convex hull edges to the edge list
  edges.clear();
  Polyhedron_3::Edge_iterator ei;
  for(ei = poly.edges_begin(); ei != poly.edges_end(); ei++)
  {
    std::cout<<ei->vertex()->index();
  }*/

#endif //__USE_CGAL3D__
}

std::vector<Edge3D> *Triangulation3D::get_edges()
{
  return &edges;
}
