#include "curve.h"

using namespace Math;

CurveSegment::CurveSegment()
{
  //int_method = INTERPOLATE_LERP;
  end_points[0].p = Float2(0.0f, 0.0f);
  end_points[1].p = Float2(1.0f, 1.0f);
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
  float x_pct = _x / (end_points[1].p[0] - end_points[0].p[0]);
  return lerp(end_points[0].p[1], end_points[1].p[1], x_pct);
}

float CurveSegmentCerp::evaluate(const float _x) const
{
  float x_pct = _x / (end_points[1].p[0] - end_points[0].p[0]);
  return cerp(end_points[0].p[1], end_points[1].p[1], x_pct);
}
