#ifndef __CURVE_H__
#define __CURVE_H__

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
      CurveEndPoint() {};
      ~CurveEndPoint() {};
    private:
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
      ~CurveSegment() {}

      virtual float evaluate(const float _x) const = 0;
    protected:
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
  };

  class Curve
  {
    public:
      Curve() {}
      ~Curve() {}

      float evaluate(const float _x);

    private:
      std::vector<CurveSegment> segments;
  };
};

#endif //__CURVE_H__
