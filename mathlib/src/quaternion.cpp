#include "quaternion.h"

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
  /*
  float oldW(w), oldX(x), oldY(y), oldZ(z);
  w = oldW*_q.w - oldX*_q.x - oldY*q.y - oldZ*_q.z;
  x = oldW*_q.x + oldX*_q.w + oldY*q.z - oldZ*_q.y;
  y = oldW*_q.y + oldY*_q.w + oldZ*q.x - oldX*_q.z;
  z = oldW*_q.z + oldZ*_q.w + oldX*q.y - oldY*_q.x;
  return *this;
  */
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

Quaternion operator/(const float &r, const Quaternion &_q)
{
  float n = _q.norm_squared();
  return Quaternion(r * _q.q[0] / n, -r * _q.q[1] / n, -r * _q.q[2] / n, -r * _q.q[3] / n);
}

ostream& operator<<(ostream &io, const Quaternion &_q)
{
  io << _q.q[0];
  (_q.q[1] < 0) ? (io << " - " << (-_q.q[1]) << "i") : (io << " + " << _q.q[1] << "i");
  (_q.q[2] < 0) ? (io << " - " << (-_q.q[2]) << "j") : (io << " + " << _q.q[2] << "j");
  (_q.q[3] < 0) ? (io << " - " << (-_q.q[3]) << "k") : (io << " + " << _q.q[3] << "k");
  return io;
}
