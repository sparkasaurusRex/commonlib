#include <iostream>
#include <assert.h>
#include "curve.h"
#include "perlin.h"

// TODO: The way I've implemented handles is particularly messy and terrible
//       The problem is that a control point / handle must be aware of end
//       points of two different curve segments. Right now a handle can also
//       be either a tangent or a position.
//
//       It seems like maybe we want different types of CurveEndPoints...
//       Bezier, corner, etc... implemented via inheritence, where each
//       corner type reports its list of handles to the UI. An endpoint
//       is more of a segment connection, then. Or maybe we implement
//       a CurveSegmentConnection container class that is responsible for
//       its handles

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

float CurveSegmentLerp::evaluate(const float _x) const
{
  float x_pct = (_x - end_points[0].p[0]) / (end_points[1].p[0] - end_points[0].p[0]);
  return clamp(lerp(end_points[0].p[1], end_points[1].p[1], x_pct), 0.0f, 1.0f);
}

float CurveSegmentCerp::evaluate(const float _x) const
{
  float x_pct = (_x - end_points[0].p[0]) / (end_points[1].p[0] - end_points[0].p[0]);
  return clamp(cerp(end_points[0].p[1], end_points[1].p[1], x_pct), 0.0f, 1.0f);
}

float CurveSegmentBezier::evaluate(const float _x) const
{
  //this code is questionable at best
  float x_pct = (_x - end_points[0].p[0]) / (end_points[1].p[0] - end_points[0].p[0]);

  float a = (1.0f - x_pct);
  float a2 = a * a;
  float a3 = a2 * a;

  Float2 b3 = a3 * end_points[0].p +
              3.0f * a2 * x_pct * end_points[0].t +
              3.0f * a * x_pct * x_pct * end_points[1].t +
              x_pct * x_pct * x_pct * end_points[1].p;

  return clamp(b3[1], 0.0f, 1.0f);
}

CurveSegmentCosine::CurveSegmentCosine() : CurveSegment()
{
  amplitude = 0.5f;
  frequency = 0.5f;
  phase = 0.0f;
  y_offset = 1.0f;
}

float CurveSegmentCosine::evaluate(const float _x) const
{
  float x_pct = (_x - end_points[0].p[0]) / (end_points[1].p[0] - end_points[0].p[0]);
  return clamp(amplitude * (cos(frequency * x_pct * M_PI * 2.0f + phase) + y_offset), 0.0f, 1.0f);
}

CurveSegmentPerlin::CurveSegmentPerlin() : CurveSegment()
{
  amplitude = 1.0f;
  frequency = 2.0f;
  phase = 0.0f;
  y_offset = 1.0f;
  octaves = 3;
}

float CurveSegmentPerlin::evaluate(const float _x) const
{
  float x_pct = (_x - end_points[0].p[0]) / (end_points[1].p[0] - end_points[0].p[0]);
  return clamp(amplitude * (PerlinNoise::octave_noise_2d(octaves, 1.0f, frequency, x_pct + phase, 0.0f) + y_offset), 0.0f, 1.0f);
}

Curve::Curve()
{
  CurveEndPoint a, b;
  a.p = Float2(0.0f, 0.0f);
  a.t = Float2(0.1f, 0.0f);
  b.p = Float2(1.0f, 1.0f);
  b.t = Float2(0.9f, 1.0f);

  CurveSegment *new_cs = new CurveSegmentLerp;
  new_cs->end_points[0] = a;
  new_cs->end_points[1] = b;

  segments.push_back(new_cs);
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

      segments.insert(csi + 1, new_cs);
      build_handle_list();
      return new_cs;
    }
  }
  return NULL;
}

void CurveHandle::translate(const Math::Float2 p)
{
  for(int i = 0; i < locations.size(); i++)
  {
    Float2 *hp = locations[i];
    assert(hp);

    *hp = p;
  }
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

void Curve::delete_handle(const CurveHandle *ch)
{
  //TODO: this is so ugly

  //need to find the curve segment(s) this handle belongs to
  std::vector<CurveSegment *> matching_segments;
  for(int i = 0; i < segments.size(); i++)
  {
    CurveSegment *cs = segments[i];
    for(int j = 0; j < 2; j++)
    {
      CurveEndPoint *cep = &cs->end_points[j];
      for(int k = 0; k < ch->locations.size(); k++)
      {
        Float2 *loc = ch->locations[k];
        if(loc == &cep->p || loc == &cep->t)
        {
          matching_segments.push_back(cs);
        }
      }
    }
  }

  //should always be 1 or 2. If not, something is wrong
  if(matching_segments.size() == 2)
  {
    CurveSegment *left = matching_segments[0];
    CurveSegment *right = matching_segments[1];

    /*if(left->end_points[0].p[0] >= right->end_points[1].p[0])
    {
      CurveSegment *tmp = left;
      left = right;
      right = tmp;
    }*/


    //delete the right one
    left->end_points[1].p = right->end_points[0].p;
    left->end_points[1].t = right->end_points[0].t;
    for(int i = 0; i < segments.size(); i++)
    {
      if(segments[i] == right)
      {
        segments.erase(segments.begin() + i);
      }
    }
    delete right;
  }

  enforce_segment_ranges();
  build_handle_list();
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

void Curve::change_segment_type(const int i, const InterpolationMethod m)
{
  CurveSegment *old_segment = segments[i];
  assert(old_segment);

  CurveSegment *new_segment = NULL;

  switch(m)
  {
    case INTERPOLATE_LERP:
      new_segment = new CurveSegmentLerp;
      break;
    case INTERPOLATE_CERP:
      new_segment = new CurveSegmentCerp;
      break;
    case INTERPOLATE_BEZIER:
      new_segment = new CurveSegmentBezier;
      break;
    case INTERPOLATE_COSINE:
      new_segment = new CurveSegmentCosine;
      break;
    case INTERPOLATE_PERLIN:
      new_segment = new CurveSegmentPerlin;
      break;
    default:
      cerr<<"Curve::change_segment_type(): unknown Interpolation Method!"<<endl;
      break;
  }

  assert(new_segment);
  new_segment->end_points[0] = old_segment->end_points[0];
  new_segment->end_points[1] = old_segment->end_points[1];

  segments[i] = new_segment;

  build_handle_list();

  delete old_segment;
}

void Curve::build_handle_list()
{
  handles.clear();

  for(int i = 0; i < segments.size(); i++)
  {
    CurveSegment *cs = segments[i];
    CurveHandle left_handle, right_handle;
    if(i == 0)
    {
      left_handle.locations.push_back(&(cs->end_points[0].p));
      handles.push_back(left_handle);
    }

    CurveSegment *right_cs = NULL;
    if(i + 1 < segments.size())
    {
      right_cs = segments[i + 1];
    }

    right_handle.locations.push_back(&(cs->end_points[1].p));
    if(right_cs)
    {
      right_handle.locations.push_back(&(right_cs->end_points[0].p));
    }
    handles.push_back(right_handle);

    if(cs->get_interpolation_method() == INTERPOLATE_BEZIER)
    {
      CurveHandle t_left, t_right;
      t_left.locations.push_back(&cs->end_points[0].t);
      t_right.locations.push_back(&cs->end_points[1].t);
      handles.push_back(t_left);
      handles.push_back(t_right);
    }
  }

  //validate
  /*for(int i = 0; i < handles.size(); i++)
  {
    cout<<i<<": "<<handles[i].locations.size()<<endl;
  }*/
}

void Curve::enforce_segment_ranges()
{
  if(segments.size() == 0) { return; }

  //enforce end points
  segments[0]->end_points[0].p[0] = 0.0f;
  segments[segments.size() - 1]->end_points[1].p[0] = 1.0f;

  for(int i = 1; i < segments.size(); i++)
  {
    CurveSegment *left = segments[i - 1];
    CurveSegment *right = segments[i];

    for(int j = 0; j < 2; j++)
    {
      for(int k = 0; k < 2; k++)
      {
        left->end_points[j].p[k] = clamp(left->end_points[j].p[k], 0.0f, 1.0f);
        left->end_points[j].t[k] = clamp(left->end_points[j].t[k], 0.0f, 1.0f);
        right->end_points[j].p[k] = clamp(right->end_points[j].p[k], 0.0f, 1.0f);
        right->end_points[j].t[k] = clamp(right->end_points[j].t[k], 0.0f, 1.0f);
      }
    }

    if(right->end_points[1].p[0] < left->end_points[1].p[0])
    {
      for(int j = 0; j < handles.size(); j++)
      {
        for(int k = 0; k < handles[j].locations.size(); k++)
        {
          if(handles[j].locations[k] == &right->end_points[1].p)
          {
            Float2 p(left->end_points[1].p[0], right->end_points[1].p[1]);
            handles[j].translate(p);
          }
        }
      }
    }
  }
}
