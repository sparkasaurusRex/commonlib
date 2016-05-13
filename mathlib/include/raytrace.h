#ifndef __RAYTRACE_H__
#define __RAYTRACE_H__

#include "math_utility.h"

namespace Math
{
  float ray_sphere_intersection(const Float3 &ray_origin, const Float3 &ray_dir, const Float3 &sphere_pos, const float radius);
};

#endif //__RAYTRACE_H__
