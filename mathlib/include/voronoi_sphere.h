#ifndef __VORONOI_SPHERE_H__
#define __VORONOI_SPHERE_H__

class VoronoiSphere
{
public:
  VoronoiSphere();
  ~VoronoiSphere();

  void add_point(Float3 p);
protected:
  int num_points;
  Float3 *points;
};

#endif __VORONOI_SPHERE_H__
