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


//3x3 matrix
Matrix3x3::Matrix3x3()
{
  identity();
}

Matrix3x3::Matrix3x3(const float _00, const float _01, const float _02,
                     const float _10, const float _11, const float _12,
                     const float _20, const float _21, const float _22)
{
    m[0][0] = _00;  m[0][1] = _01;  m[0][2] = _02;
    m[1][0] = _10;  m[1][1] = _11;  m[1][2] = _12;
    m[2][0] = _20;  m[2][1] = _21;  m[2][2] = _22;
}

Float3 Matrix3x3::operator*(const Float3 &r) const
{
  Float3 ret;
  ret[0] = m[0][0] * r[0] + m[0][1] * r[1] + m[0][2] * r[2];
  ret[1] = m[1][0] * r[0] + m[1][1] * r[1] + m[1][2] * r[2];
  ret[2] = m[2][0] * r[0] + m[2][1] * r[1] + m[2][2] * r[2];
  return ret;
}

void Matrix3x3::identity()
{
  m[0][0] = 1.0f;   m[0][1] = 0.0f;   m[0][2] = 0.0f;
  m[1][0] = 0.0f;   m[1][1] = 1.0f;   m[1][2] = 0.0f;
  m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = 1.0f;
}

void Matrix3x3::rotation_from_quaternion(const Quaternion &q)
{
  float x2 = q.q[1] * q.q[1];
  float y2 = q.q[2] * q.q[2];
  float z2 = q.q[3] * q.q[3];
  float w2 = q.q[0] * q.q[0];

  float two_x = 2.0f * q.q[1];
  float two_y = 2.0f * q.q[2];
  float two_w = 2.0f * q.q[0];

  float xy = two_x * q.q[2];
  float xz = two_x * q.q[3];
  float yz = two_y * q.q[3];
  float wx = two_w * q.q[1];
  float wy = two_w * q.q[2];
  float wz = two_w * q.q[3];

  m[0][0] = w2 + x2 - y2 - z2;
  m[0][1] = xy - wz;
  m[0][2] = xz + wy;

  m[1][0] = xy + wz;
  m[1][1] = w2 - x2 + y2 - z2;
  m[1][2] = yz - wx;

  //fill in the third row
  m[2][0] = xz - wy;
  m[2][1] = yz + wx;
  m[2][2] = w2 - x2 - y2 + z2;
}



ostream& Math::operator<<(ostream &os, const Matrix2x2 &m)
{
  os<<m(0, 0)<<" "<<m(0, 1)<<endl;
  os<<m(1, 0)<<" "<<m(1, 1)<<endl;

  return os;
}

ostream& Math::operator<<(ostream &os, const Matrix3x3 &m)
{
  os<<m(0, 0)<<" "<<m(0, 1)<<" "<<m(0, 2)<<endl;
  os<<m(1, 0)<<" "<<m(1, 1)<<" "<<m(1, 2)<<endl;
  os<<m(2, 0)<<" "<<m(2, 1)<<" "<<m(2, 2)<<endl;

  return os;
}
