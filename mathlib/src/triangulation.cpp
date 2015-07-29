#if USE_CGAL

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Delaunay_triangulation_2.h>

#endif //USE_CGAL

#include <assert.h>
#include <iostream>
#include "triangulation.h"

using namespace Math;
using namespace std;

#if USE_CGAL
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef Delaunay::Face_iterator FaceIterator;
typedef Delaunay::Vertex_handle VertexHandle;
#endif

Triangulation2D::Triangulation2D() { vertices = NULL; }

Triangulation2D::~Triangulation2D() {}

void Triangulation2D::set_vertices(std::vector<Float2> *verts)
{
  assert(verts);
  vertices = verts;
}

bool sort_compare_x(Float2 a, Float2 b)
{
  //if(a[0] == b[0]) { return a[1] < b[1]; }
  return a[0] < b[0];
}

bool sort_compare_y(Float2 a, Float2 b)
{
  return a[1] < b[1];
}

void Triangulation2D::generate_delaunay_triangulation()
{
  assert(vertices);
  triangles.clear();

  //first, sort the vertices by x-coord, w/ y-coord for tie breakers
  //std::sort(vertices->begin(), vertices->end(), sort_compare_x);
  delaunay_divide_and_conquer(0, 0, vertices->size());

  //delaunay_cgal();
}

#if USE_CGAL
void Triangulation2D::delaunay_cgal()
{
  Delaunay dt;
  for(int i = 0; i < vertices->size(); i++)
  {
    Float2 vert = (*vertices)[i];
    Point_2 p(vert[0], vert[1]);
    cout<<"cgal - adding vertex "<<vert<<endl;
    dt.push_back(p);
  }
  FaceIterator fi = dt.faces_begin();

  cout<<"foo"<<endl;
  for(; fi != dt.faces_end(); fi++)
  {
    Triangle2D tri;
    for(int i = 0; i < 3; i++)
    {
      VertexHandle vh = (*fi).vertex(i);
      tri.indices[i] = (*fi).index(vh);

      cout<<tri.indices[i]<<endl;
    }
    //tri.indices[0] = fi[0];
    triangles.push_back(tri);
  }
}
#endif //USE_CGAL

void Triangulation2D::delaunay_merge(int start_a, int end_a, int start_b, int end_b)
{
  Triangle2D t;
  t.indices[0] = end_a - 1;
  t.indices[1] = end_a;
  t.indices[2] = start_b;

  triangles.push_back(t);
}

void Triangulation2D::delaunay_divide_and_conquer(int axis, int start_idx, int end_idx)
{
  std::vector<Float2>::iterator start = vertices->begin() + start_idx;
  std::vector<Float2>::iterator end = vertices->begin() + end_idx;
  if(axis == 0)
  {
    std::sort(start, end, sort_compare_x);
  }
  else
  {
    std::sort(start, end, sort_compare_y);
  }

  //find the mean vertex
  int num_verts = (end_idx - start_idx);
  int mean_idx = num_verts / 2;
  Float2 mean_vertex = (*vertices)[start_idx + mean_idx];

  cout<<"num verts: "<<num_verts<<endl;
  cout<<"mean index: "<<mean_idx<<endl;
  cout<<"mean vertex: "<<mean_vertex<<endl;

  if(num_verts >= 4)
  {
    delaunay_divide_and_conquer(axis, start_idx, start_idx + mean_idx);
    delaunay_divide_and_conquer(axis, start_idx + mean_idx, end_idx);
    delaunay_merge(start_idx, start_idx + mean_idx, start_idx + mean_idx, end_idx);
  }
  else if (num_verts == 3)
  {
    Triangle2D t;
    t.indices[0] = start_idx;
    t.indices[1] = start_idx + 1;
    t.indices[2] = start_idx + 2;
    triangles.push_back(t);
  }
}

std::vector<Triangle2D> *Triangulation2D::get_triangles()
{
  return &triangles;
}
