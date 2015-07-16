#include "aabb.h"

using namespace Math;

AABB2D::AABB2D(const Float2 pt)
{
  max_pt = min_pt = pt;
}

Float2 AABB2D::dim() const { return max_pt - min_pt; }

void AABB2D::grow(const Float2 p)
{
  min_pt[0] = min(min_pt[0], p[0]);
  max_pt[0] = max(max_pt[0], p[0]);

  min_pt[1] = min(min_pt[1], p[1]);
  max_pt[1] = max(max_pt[1], p[1]);
}

int AABB2D::get_min_axis() const
{
  Float2 d = max_pt - min_pt;
  if(d[0] < d[1]) { return 0; }
  return 1;
}

int AABB2D::get_max_axis() const
{
  Float2 d = max_pt - min_pt;
  if(d[0] > d[1]) { return 0; }
  return 1;
}


//
//3d axis-aligned bounding box
//
AABB3D::AABB3D(const Float3 pt)
{
  max_pt = min_pt = pt;
}

Float3 AABB3D::dim() const { return max_pt - min_pt; }

void AABB3D::grow(const Float3 p)
{
  for(int i = 0; i < 3; i++)
  {
    min_pt[i] = min(min_pt[i], p[i]);
    max_pt[i] = max(max_pt[i], p[i]);
  }
}

int AABB3D::get_min_axis() const
{
  Float3 d = max_pt - min_pt;
  if(d[0] < d[1] && d[0] < d[2]) { return 0; }
  else if(d[1] < d[0] && d[1] < d[2]) { return 1; }
  return 2;
}

int AABB3D::get_max_axis() const
{
  Float3 d = max_pt - min_pt;
  if(d[0] > d[1] && d[0] > d[2]) { return 0; }
  else if(d[1] > d[0] && d[1] > d[2]) { return 1; }
  return 2;
}
