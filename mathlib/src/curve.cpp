#include <iostream>
#include "curve.h"

using namespace Math;
using namespace std;

CurveSegment::CurveSegment()
{
  //int_method = INTERPOLATE_LERP;
  end_points[0].p = Float2(0.0f, 0.0f);
  end_points[1].p = Float2(1.0f, 1.0f);
}

void CurveSegment::set_endpoints(CurveEndPoint &a, CurveEndPoint &b)
{
  end_points[0] = a;
  end_points[1] = b;
}

bool CurveSegment::in_range(const float _x) const
{
  if((_x >= end_points[0].p[0]) && (_x <= end_points[1].p[0]))
  {
    return true;
  }
  return false;
}

//cleaner / faster - use inheritance instead?
/*float CurveSegment::evaluate(const float _x)
{
  float x_pct = _x  / (end_points[1].p[0] - end_points[0].p[0])
  switch(int_method)
  {
    case INTERPOLATE_LERP:
      return lerp(end_points[0].p[1], end_points[1].p[1], _x);
      break;
    case INTERPOLATE_CERP:
      return cerp(end_points[0].p[1], end_points[1].p[1], _x);
      break;
    default:
      assert(false);
      break;
  }
}*/

float CurveSegmentLerp::evaluate(const float _x) const
{
  float x_pct = (_x - end_points[0].p[0]) / (end_points[1].p[0] - end_points[0].p[0]);
  return lerp(end_points[0].p[1], end_points[1].p[1], x_pct);
}

float CurveSegmentCerp::evaluate(const float _x) const
{
  float x_pct = (_x - end_points[0].p[0]) / (end_points[1].p[0] - end_points[0].p[0]);
  //cout<<"x_pct: "<<x_pct<<endl;
  return cerp(end_points[0].p[1], end_points[1].p[1], x_pct);
}

void Curve::add_segment(CurveSegment *s)
{
  segments.push_back(s);
}

float Curve::evaluate(const float _x)
{
  for(int i = 0; i < segments.size(); i++)
  {
    if(segments[i]->in_range(_x))
    {
      return segments[i]->evaluate(_x);
    }
  }
  return 0.0f;
}
