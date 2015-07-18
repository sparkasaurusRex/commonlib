#include <math.h>
#include "quaternion.h"
#include "math_utility.h"

using namespace Math;

Quaternion::Quaternion()
{
  q[0] = q[1] = q[2] = q[3] = 0.0f;
}


Quaternion::Quaternion(const float &w, const float &x, const float &y, const float &z)
{
  q[0] = w;
  q[1] = x;
  q[2] = y;
  q[3] = z;
}

Quaternion::Quaternion(const float &x, const float &y, const float &z)
{
  q[0] = 0;
  q[1] = x;
  q[2] = y;
  q[3] = z;
}

Quaternion::Quaternion(const float &r)
{
  q[0] = r;
  q[1] = q[2] = q[3] = 0.0f;
}


Quaternion::Quaternion(const Quaternion &_q)
{
  for(int i = 0; i < 4; i++)
  {
    q[i] = _q.q[i];
  }
}

Quaternion& Quaternion::operator=(const Quaternion &_q)
{
  for(int i = 0; i < 4; i++)
  {
    q[i] = _q.q[i];
  }
  return *this;
}


Quaternion Quaternion::operator-() const
{
  return Quaternion(-q[0], -q[0], -q[0], -q[0]);
}

// Conjugate
Quaternion Quaternion::operator~() const
{
  return Quaternion(q[0], -q[1], -q[2], -q[3]);
}


float Quaternion::norm_squared() const
{
  return q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
}

void Quaternion::normalize()
{
  float n = sqrt(norm_squared());
  q[0] /= n; q[1] /= n; q[2] /= n; q[3] /= n;
}

void Quaternion::rotation_from_axis_angle(const Float3 &norm_axis, const float theta_rad)
{
  //this function assumes that norm_axis is normalized!!!
  float phi = theta_rad / 2.0f;
  float sin_phi = sin(phi);
  q[0] = cos(phi);
  q[1] = norm_axis[0] * sin_phi;
  q[2] = norm_axis[1] * sin_phi;
  q[3] = norm_axis[2] * sin_phi;
}

Quaternion& Quaternion::operator+=(const Quaternion &_q)
{
  for(int i = 0; i < 4; i++)
  {
    q[i] += _q.q[i];
  }
  return *this;
}

Quaternion& Quaternion::operator+=(const float &r)
{
  q[0] += r; return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion &_q)
{

  float old_val[4];
  for(int i = 0; i < 4; i++)
  {
    old_val[i] = q[i];
  }

  q[0] = old_val[0] * _q.q[0] - old_val[1] * _q.q[1] - old_val[2] * _q.q[2] - old_val[3] * _q.q[3];
  q[1] = old_val[0] * _q.q[1] + old_val[1] * _q.q[0] + old_val[2] * _q.q[3] - old_val[3] * _q.q[2];
  q[2] = old_val[0] * _q.q[2] + old_val[2] * _q.q[0] + old_val[3] * _q.q[1] - old_val[1] * _q.q[3];
  q[3] = old_val[0] * _q.q[3] + old_val[3] * _q.q[0] + old_val[1] * _q.q[2] - old_val[2] * _q.q[1];

  return *this;
}

Quaternion& Quaternion::operator/=(const Quaternion &_q)
{
  float old_val[4];
  float n = _q.norm_squared();
  for(int i = 0; i < 4; i++)
  {
    old_val[i] = q[i];
  }
  q[0] = (old_val[0] *_q.q[0] + old_val[1] * _q.q[1] + old_val[2] * _q.q[2] + old_val[3] * _q.q[3]) / n;
  q[1] = (old_val[1] *_q.q[0] - old_val[0] * _q.q[1] + old_val[2] * _q.q[3] - old_val[3] * _q.q[2]) / n;
  q[2] = (old_val[2] *_q.q[0] - old_val[0] * _q.q[2] + old_val[3] * _q.q[1] - old_val[1] * _q.q[3]) / n;
  q[3] = (old_val[3] *_q.q[0] - old_val[0] * _q.q[3] + old_val[1] * _q.q[2] - old_val[2] * _q.q[1]) / n;
  return *this;
}

Quaternion Math::operator+(const float &r, const Quaternion &_q)
{
  return _q + r;
}

Quaternion Math::operator-(const float &r, const Quaternion &_q)
{
  return Quaternion(r - _q.q[0], _q.q[1], _q.q[2], _q.q[3]);
}

Quaternion Math::operator*(const float &r, const Quaternion &_q)
{
  return _q * r;
}

Quaternion Math::operator/(const float &r, const Quaternion &_q)
{
  float n = _q.norm_squared();
  return Quaternion(r * _q.q[0] / n, -r * _q.q[1] / n, -r * _q.q[2] / n, -r * _q.q[3] / n);
}

ostream& Math::operator<<(ostream &io, const Quaternion &_q)
{
  io << _q.q[0];
  (_q.q[1] < 0) ? (io << " - " << (-_q.q[1]) << "i") : (io << " + " << _q.q[1] << "i");
  (_q.q[2] < 0) ? (io << " - " << (-_q.q[2]) << "j") : (io << " + " << _q.q[2] << "j");
  (_q.q[3] < 0) ? (io << " - " << (-_q.q[3]) << "k") : (io << " + " << _q.q[3] << "k");
  return io;
}
