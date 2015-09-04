#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <vector>
#include "math_utility.h"

namespace Math
{
  enum PointType
  {
    POINT_TYPE_LINEAR,
    POINT_TYPE_BEZIER,
    NUM_VERTEX_TYPES
  };

  class FunctionPoint
  {
    public:
      FunctionPoint();

      Float2 pos;
      Float2 tangent;

      PointType point_type;
  };

  enum CurveType
  {
      CURVE_TYPE_LINEAR,
      CURVE_TYPE_CERP,
      CURVE_TYPE_BEZIER,
      CURVE_TYPE_PERIODIC,
      NUM_CURVE_TYPES
  };

  class FunctionCurve
  {
    public:
      FunctionCurve();
      ~FunctionCurve() {}

      float evaluate(const float x) const;
    private:
      FunctionPoint v[2];
  };

  class Function
  {
    public:
      Function();
      ~Function();

      float evaluate(const float x) const;
    private:

      CurveType curve_type;
      std::vector<FunctionCurve> verts;
  };
};

#endif // __FUNCTION_H__
