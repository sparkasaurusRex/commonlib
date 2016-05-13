#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include "math_utility.h"

//#define M_PI 3.14159f

using namespace Math;

Float2::Float2()
{
    _val[0] = _val[1] = 0.0f;
}

Float2::Float2(const float _x, const float _y)
{
    _val[0] = _x;
    _val[1] = _y;
}

Float2::Float2(const Float2 &b)
{
    _val[0] = b._val[0];
    _val[1] = b._val[1];
}

Float3::Float3()
{
    _val[0] = _val[1] = _val[2];
}

Float3::Float3(const float _x, const float _y, const float _z)
{
    _val[0] = _x;
    _val[1] = _y;
    _val[2] = _z;
}

Float3::Float3(const Float2 &b, const float _z = 0.0f)
{
    _val[0] = b._val[0];
    _val[1] = b._val[1];
    _val[2] = _z;
}

Float3::Float3(const Float3 &b)
{
    _val[0] = b._val[0];
    _val[1] = b._val[1];
    _val[2] = b._val[2];
}

/*float Math::cerp(float y1, float y2, float mu)
{
   double mu2;

   mu2 = (1.0f - cos(mu * M_PI)) / 2.0f;
   return (y1  *(1.0f - mu2) + y2 * mu2);
}
*/

/*float Math::lerp(float y1, float y2, float mu)
{
   return (y1 * (1.0f - mu) + y2 * mu);
}*/

float Math::clamp(float x, float a, float b)
{
	register float low = std::min(a, b);
	register float hi = std::max(a, b);
	if(x < low) x = low;
	if(x > hi) x = hi;
	return x;
}

#if 0
/*inline */float Math::max(float a, float b)
{
	return (a > b) ? a : b;
}

/*inline*/ float Math::min(float a, float b)
{
	return (a < b) ? a : b;
}
#endif

float Math::random(float a, float b)
{
	return a + (b - a) * ((rand() % RAND_MAX) / (float)RAND_MAX);
}

int Math::random(int a, int b)
{
	return a + rand() % abs(b - a);
}

/*
float Math::degrees_to_radians(float d)
{
	return (d / 180.0f) * M_PI;
}


float Math::radians_to_degrees(float r)
{
	return (r / M_PI) * 180.0f;
}*/

unsigned int Math::hash(const char *str)
{
  unsigned int hash = 5381;
  int c;

  while((c = *str++))
  {
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash;
}
