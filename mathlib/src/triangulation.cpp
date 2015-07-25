#include <assert.h>
#include "triangulation.h"

using namespace Math;

Triangulation2D::Triangulation2D() { vertices = NULL; }

Triangulation2D::~Triangulation2D() {}

void Triangulation2D::set_vertices(std::vector<Float2> *verts)
{
  assert(verts);
  vertices = verts;
}

void Triangulation2D::generate_delaunay_triangulation()
{
  //test bad bad bad
  assert(vertices);
  if(vertices)
  {
    for(int i = 0; i < vertices->size(); i += 3)
    {
      if(i + 3 < vertices->size())
      {
        Triangle2D t;
        t.indices[0] = i;
        t.indices[1] = i + 1;
        t.indices[2] = i + 2;

        triangles.push_back(t);
      }
    }
  }
}

std::vector<Triangle2D> *Triangulation2D::get_triangles()
{
  return &triangles;
}
