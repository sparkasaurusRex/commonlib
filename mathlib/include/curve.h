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
    INTERPOLATE_COSINE,
    INTERPOLATE_PERLIN,
    NUM_INTERPOLATION_METHODS
  };

  class CurveEndPoint
  {
    friend class CurveSegment;
    public:
      CurveEndPoint() {}
      ~CurveEndPoint() {}
  //  private:
      Float2 p;   //vertex
      Float2 t;   //tangent handle
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

  //
  // cosine periodic
  //
  class CurveSegmentCosine : public CurveSegment
  {
  public:
    CurveSegmentCosine();
    ~CurveSegmentCosine() {}

    virtual float evaluate(const float _x) const;
    virtual InterpolationMethod get_interpolation_method() const { return INTERPOLATE_COSINE; }

    float amplitude;
    float frequency;
    float phase;
    float y_offset;
  };

  //
  // perlin periodic
  //
  class CurveSegmentPerlin : public CurveSegment
  {
  public:
    CurveSegmentPerlin();
    ~CurveSegmentPerlin() {}

    virtual float evaluate(const float _x) const;
    virtual InterpolationMethod get_interpolation_method() const { return INTERPOLATE_PERLIN; }

    float amplitude;
    float frequency;
    float phase;
    float y_offset;
    int octaves;
  };

  class CurveHandle
  {
    public:
      CurveHandle() {}
      ~CurveHandle() {}

      void translate(const Math::Float2 p);
      Float2 get_pos() { return *(locations[0]); }
      std::vector<Math::Float2 *> locations;
  };

  class Curve
  {
    public:
      Curve();
      ~Curve();

      void add_segment(CurveSegment *s);
      CurveSegment *create_segment(InterpolationMethod m, CurveEndPoint new_a, CurveEndPoint new_b);

      float evaluate(const float _x);

      int get_num_segments() const { return segments.size(); }
      CurveSegment *get_segment_by_index(const int i) { return segments[i]; }

      int get_num_handles() const { return handles.size(); }
      CurveHandle *get_handle_by_index(const int i) { return &handles[i]; }
      void delete_handle(const CurveHandle *ch);

      CurveSegment *get_segment(const float x);

      void change_segment_type(const int i, const InterpolationMethod m);

      void enforce_segment_ranges();
    private:
      void build_handle_list();

      std::vector<CurveSegment *>   segments;
      std::vector<CurveHandle>      handles;
  };
};

#endif //__CURVE_H__
