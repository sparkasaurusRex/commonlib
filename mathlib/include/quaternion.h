#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include <iostream>
using namespace std;

class Quaternion
{
public:
  float q[4];// w, x, y, z;

  // Numerical constructor
  Quaternion(const float &w, const float &x, const float &y, const float &z);
  Quaternion(const float &x, const float &y, const float &z); // For 3-rotations
  Quaternion(const float &r);
  Quaternion();

  // Copy constructor and assignment
  Quaternion(const Quaternion &_q);
  Quaternion& operator=(const Quaternion &_q);

  // Unary operators
  Quaternion operator-() const;
  Quaternion operator~() const; // Conjugate

  // Norm-squared. SQRT would have to be made generic to be used here
  float norm_squared() const;

  // In-place operators
  Quaternion& operator+=(const float &r);
  Quaternion& operator+=(const Quaternion &_q);

  Quaternion& operator-=(const float &r)
    { q[0] -= r; return *this; }
  Quaternion& operator-=(const Quaternion &_q)
    { q[0] -= _q.q[0]; q[1] -= _q.q[1]; q[2] -= _q.q[2]; q[3] -= _q.q[3]; return *this; }

  Quaternion& operator*=(const float &r)
    { q[0] *= r; q[1] *= r; q[2] *= r; q[3] *= r; return *this; }
  Quaternion& operator*=(const Quaternion &_q);

  Quaternion& operator/=(const float &r)
    { q[0] /= r; q[1] /= r; q[2] /= r; q[3] /= r; return *this; }
  Quaternion& operator/=(const Quaternion &q);

  // Binary operators based on in-place operators
  Quaternion operator+(const float &r) const { return Quaternion(*this) += r; }
  Quaternion operator+(const Quaternion &_q) const { return Quaternion(*this) += _q; }
  Quaternion operator-(const float &r) const { return Quaternion(*this) -= r; }
  Quaternion operator-(const Quaternion &_q) const { return Quaternion(*this) -= _q; }
  Quaternion operator*(const float &r) const { return Quaternion(*this) *= r; }
  Quaternion operator*(const Quaternion &_q) const { return Quaternion(*this) *= _q; }
  Quaternion operator/(const float &r) const { return Quaternion(*this) /= r; }
  Quaternion operator/(const Quaternion &_q) const { return Quaternion(*this) /= _q; }

  // Comparison operators, as much as they make sense
  bool operator==(const Quaternion &_q) const
    { return (q[0] == _q.q[0]) && (q[1] == _q.q[1]) && (q[2] == _q.q[2]) && (q[3] == _q.q[3]); }
  bool operator!=(const Quaternion &_q) const { return !operator==(_q); }

  // The operators above allow quaternion op real. These allow real op quaternion.
  // Uses the above where appropriate.
  friend Quaternion operator+(const float &r, const Quaternion &_q);
  friend Quaternion operator-(const float &r, const Quaternion &_q);
  friend Quaternion operator*(const float &r, const Quaternion &_q);
  friend Quaternion operator/(const float &r, const Quaternion &_q);

  // Allows cout << q
  friend ostream& operator<<(ostream &io, const Quaternion &_q);
};

Quaternion operator+(const float &r, const Quaternion &_q) { return _q + r; }
Quaternion operator-(const float &r, const Quaternion &_q) { return Quaternion(r - _q.q[0], _q.q[1], _q.q[2], _q.q[3]); }
Quaternion operator*(const float &r, const Quaternion &_q) { return _q * r; }

Quaternion operator/(const float &r, const Quaternion &_q);
ostream& operator<<(ostream &io, const Quaternion &_q);


#endif //__QUATERNION_H__
