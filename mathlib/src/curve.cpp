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

float CurveSegmentBezier::evaluate(const float _x) const
{
  float x_pct = (_x - end_points[0].p[0]) / (end_points[1].p[0] - end_points[0].p[0]);

  float a = (1.0f - x_pct);
  float a2 = a * a;
  float a3 = a2 * a;

  Float2 b3 = a3 * end_points[0].p +
              3.0f * a2 * x_pct * end_points[0].t +
              3.0f * a * x_pct * x_pct * end_points[1].t +
              x_pct * x_pct * x_pct * end_points[1].p;

  return b3[1];
}

Curve::Curve()
{
  create_segment(INTERPOLATE_CERP, Float2(0.0f, 1.0f));
}

Curve::~Curve()
{
  for(int i = 0; i < segments.size(); i++)
  {
    delete segments[i];
  }
}

void Curve::add_segment(CurveSegment *s)
{
  cerr<<"Curve::add_segment(): DANGEROUS!!! DON'T USE!!!"<<endl;
  segments.push_back(s);
}

CurveSegment *Curve::create_segment(InterpolationMethod m, Math::Float2 range_x)
{
  CurveSegment *cs = NULL;
  switch(m)
  {
    case INTERPOLATE_LERP:
      cs = new CurveSegmentLerp;
      break;
    case INTERPOLATE_CERP:
      cs = new CurveSegmentCerp;
      break;
    case INTERPOLATE_BEZIER:
      cs = new CurveSegmentBezier;
      break;
    default:
      cerr<<"Curve::create_segment(): unknown interpolation method!"<<endl;
      return NULL;
      break;
  }

  if(cs)
  {
    cs->end_points[0].p[0] = range_x[0];
    cs->end_points[1].p[0] = range_x[1];
    segments.push_back(cs);
    return cs;
  }
  return NULL;
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

CurveSegment *Curve::get_segment(const float x)
{
  for(int i = 0; i < segments.size(); i++)
  {
    CurveSegment *cs = segments[i];
    if(cs->end_points[0].p[0] <= x && cs->end_points[1].p[0] >= x)
    {
      return cs;
    }
  }
  return NULL;
}
