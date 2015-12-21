#include "voronoi_sphere.h"

using namespace Math;

VoronoiSphere::VoronoiSphere()
{
}

VoronoiSphere::~VoronoiSphere() {}

void VoronoiSphere::add_point(const Float3 &p)
{
  pc.push_back(p);
}

Float3 VoronoiSphere::get_point(const int i) const
{
  return pc[i];
}

void VoronoiSphere::triangulate()
{
  delaunay.generate_delaunay_triangulation();
}

void VoronoiSphere::triangulation_step(const float dh)
{
  delaunay.advance_beach_line(dh);
  delaunay.fortune_step();
}

TriangulationSphere *VoronoiSphere::get_triangulation()
{
  return &delaunay;
}
