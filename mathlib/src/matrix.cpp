#include "matrix.h"

// look into using an optimized linear algebra library (usually waaaaay faster)
// http://arma.sourceforge.net/download.html
// http://www.openblas.net/

using namespace Math;

Matrix2x2::Matrix2x2()
{
  m[0][0] = 1.0f;   m[0][1] = 0.0f;
  m[1][0] = 0.0f;   m[1][1] = 1.0f;
}

Matrix2x2::Matrix2x2(const float _00, const float _01, const float _10, const float _11)
{
  m[0][0] = _00;    m[0][1] = _01;
  m[1][0] = _10;    m[1][1] = _11;
}

Matrix2x2 Matrix2x2::operator*(const Matrix2x2 &r)
{
  return Matrix2x2(m[0][0] * r.m[0][0] + m[0][1] * r.m[1][0],
                   m[0][0] * r.m[0][1] + m[0][1] * r.m[1][1],
                   m[1][0] * r.m[0][0] + m[1][1] * r.m[1][0],
                   m[1][0] * r.m[0][1] + m[1][1] * r.m[1][1]);
}

Float2 Matrix2x2::operator*(const Float2 &r)
{
  return Float2(m[0][0] * r[0] + m[0][1] * r[1], m[1][0] * r[0] + m[1][1] * r[1]);
}

void Matrix2x2::identity()
{
  m[0][0] = 1.0f;   m[0][1] = 0.0f;
  m[1][0] = 0.0f;   m[1][1] = 1.0f;
}

void Matrix2x2::rotation(const float rad)
{
  float cos_theta = cos(rad);
  float sin_theta = sin(rad);
  m[0][0] = cos_theta;   m[0][1] = -sin_theta;
  m[1][0] = sin_theta;   m[1][1] = cos_theta;
}
