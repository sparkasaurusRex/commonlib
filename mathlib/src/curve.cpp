#include <iostream>
#include <assert.h>
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
  CurveEndPoint a, b;
  a.p = Float2(0.0f, 0.0f);
  a.t = Float2(0.1f, 0.0f);
  b.p = Float2(1.0f, 1.0f);
  b.t = Float2(0.9f, 1.0f);

  CurveSegment *new_cs = new CurveSegmentCerp;
  new_cs->end_points[0] = a;
  new_cs->end_points[1] = b;

  segments.push_back(new_cs);

  //create_segment(INTERPOLATE_CERP, a, b);
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

CurveSegment *Curve::create_segment(InterpolationMethod m, CurveEndPoint new_a, CurveEndPoint new_b)
{
  CurveSegment *new_cs = NULL;
  switch(m)
  {
    case INTERPOLATE_LERP:
      new_cs = new CurveSegmentLerp;
      break;
    case INTERPOLATE_CERP:
      new_cs = new CurveSegmentCerp;
      break;
    case INTERPOLATE_BEZIER:
      new_cs = new CurveSegmentBezier;
      break;
    default:
      cerr<<"Curve::create_segment(): unknown interpolation method!"<<endl;
      return NULL;
      break;
  }

  // Assumptions:
  //
  //  1. existing list of segments is already sorted and no segments
  //     overlap on the x-axis
  //  2. insertion segment doesn't overlap existing segments, and
  //     is a valid range to begin with
  //
  // Goals:
  //
  //  1. insert new segment in correct order
  //  2. magnet endpoints together
  //  3. regnerate list of curve handles
  std::vector<CurveSegment *>::iterator csi;
  for(csi = segments.begin(); csi != segments.end(); csi++)
  {
    CurveSegment *old_cs = *csi;
    CurveEndPoint *end_a = &(old_cs->end_points[0]);
    CurveEndPoint *end_b = &(old_cs->end_points[1]);

    if(new_a.p[0] >= end_a->p[0] && new_b.p[0] <= end_b->p[0])
    {
      new_cs->end_points[0] = new_a;
      new_cs->end_points[1] = new_b;
      segments.insert(csi, new_cs);
      return new_cs;
    }
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

void CurveHandle::translate(const Math::Float2 p, bool move_tangents)
{
  for(int i = 0; i < end_points.size(); i++)
  {
    CurveEndPoint *ep = end_points[i];
    assert(ep);
    if(move_tangents)
    {
      ep->t = p;
    }
    else
    {
      ep->p = p;
    }
  }
}
