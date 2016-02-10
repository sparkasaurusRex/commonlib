#ifndef __CURVE_H__
#define __CURVE_H__

#include <vector>
#include "math_utility.h"

namespace Math {
  enum InterpolationMethod
  {
    INTERPOLATE_LERP,
    INTERPOLATE_CERP,
    INTERPOLATE_BEZIER,
    NUM_INTERPOLATION_METHODS
  };

  class CurveEndPoint
  {
    friend class CurveSegment;
    public:
      CurveEndPoint() { neighbor = NULL; }
      ~CurveEndPoint() {}
  //  private:
      Float2 p;   //vertex
      Float2 t;   //tangent handle

      CurveEndPoint *neighbor;
  };

  //
  // base class for interpolation methods
  //
  class CurveSegment
  {
    friend class Curve;
    public:
      CurveSegment();
      ~CurveSegment() {};

      virtual float evaluate(const float _x) const = 0;
      virtual InterpolationMethod get_interpolation_method() const = 0;

      void set_endpoints(CurveEndPoint &a, CurveEndPoint &b);

      bool in_range(const float _x) const;
    //protected:
      CurveEndPoint end_points[2];
  };

  //
  // linear interpolation
  //
  class CurveSegmentLerp : public CurveSegment
  {
    public:
      CurveSegmentLerp() : CurveSegment() {}
      ~CurveSegmentLerp() {}
      virtual float evaluate(const float _x) const;
      virtual InterpolationMethod get_interpolation_method() const { return INTERPOLATE_LERP; }
  };

  //
  // cosine interpolation
  //
  class CurveSegmentCerp : public CurveSegment
  {
  public:
    CurveSegmentCerp() : CurveSegment() {}
    ~CurveSegmentCerp() {}
    virtual float evaluate(const float _x) const;
    virtual InterpolationMethod get_interpolation_method() const { return INTERPOLATE_CERP; }
  };

  //
  // bezier (cubic) interpolation
  //
  class CurveSegmentBezier : public CurveSegment
  {
  public:
    CurveSegmentBezier() : CurveSegment() {}
    ~CurveSegmentBezier() {}
    virtual float evaluate(const float _x) const;
    virtual InterpolationMethod get_interpolation_method() const { return INTERPOLATE_BEZIER; }
  };

  class Curve
  {
    public:
      Curve();
      ~Curve();

      void add_segment(CurveSegment *s);
      CurveSegment *create_segment(InterpolationMethod m, Math::Float2 range_x);

      float evaluate(const float _x);

      int get_num_segments() const { return segments.size(); }
      CurveSegment *get_segment_by_index(const int i) { return segments[i]; }

      CurveSegment *get_segment(const float x);

    private:
      std::vector<CurveSegment *> segments;
  };
};

#endif //__CURVE_H__
