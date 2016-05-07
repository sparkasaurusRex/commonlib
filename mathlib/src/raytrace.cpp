#include "raytrace.h"

using namespace Math;

float Math::ray_sphere_intersection(const Float3 &ray_origin, const Float3 &ray_dir, const Float3 &sphere_pos, const float radius)
{
  Float3 q = sphere_pos - ray_origin;
  float c = q.magnitude();
  float v = q * ray_dir; //dot product
  float d = radius * radius - (c * c - v * v);
  
  if (d < 0.0f) { return -1.0f;  }

  return v - sqrt(d);
}
