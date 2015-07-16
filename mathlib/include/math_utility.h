#ifndef _MATH_UTILITY_H_
#define _MATH_UTILITY_H_

#include <math.h>
#include <iostream>

#ifndef M_PHI
#define M_PHI   1.61803398874989484820f
#endif

namespace Math {
  class Float2 {
    public:
      Float2();
      Float2(const float _x, const float _y);
      Float2(const Float2 &b);

      inline Float2 &operator=(const Float2 &r) { _val[0] = r._val[0]; _val[1] = r._val[1]; return *this; }
      inline Float2 operator+(const Float2 &r) { return Float2(_val[0] + r._val[0], _val[1] + r._val[1]); }
      inline Float2 operator-(const Float2 &r) { return Float2(_val[0] - r._val[0], _val[1] - r._val[1]); }
      inline Float2 operator/(const float &r) { return Float2(_val[0] / r, _val[1] / r); }
      inline Float2 operator*(const float &r) const { return Float2(r * _val[0], r * _val[1]); }

      inline float &operator[](const int idx) { return _val[idx]; }
      inline float operator[](const int idx) const { return _val[idx]; }

      //dot product
      inline float operator*(const Float2 &b) const { return b._val[0] * _val[0] + b._val[1] * _val[1]; }

      inline float x() const { return _val[0]; }
      inline float y() const { return _val[1]; }

      //swizzle
      //inline Float2 xy() const { return Float2(_val[0], _val[1]); }
      inline Float2 yx() const { return Float2(_val[1], _val[0]); }

      inline void normalize() { float m = magnitude(); _val[0] /= m; _val[1] /= m; }
      inline float magnitude() const { return sqrt(_val[0] * _val[0] + _val[1] * _val[1]); }
      inline float mag_squared() const { return _val[0] * _val[0] + _val[1] * _val[1]; }
      //inline float distance(Float2 p) { return magnitude(this - p); }

      //data
      float _val[2];
  };

    class Float3 {
        public:
            Float3();
            Float3(const float _x, const float _y, const float _z);
            Float3(const Float2 &b, const float _z);
            Float3(const Float3 &b);

            inline Float3 &operator=(const Float3 &r) { _val[0] = r._val[0]; _val[1] = r._val[1]; _val[2] = r._val[2]; return *this; }
            inline Float3 operator+(const Float3 &r) { return Float3(_val[0] + r._val[0], _val[1] + r._val[1], _val[2] + r._val[2]); }
            inline Float3 operator-(const Float3 &r) { return Float3(_val[0] - r._val[0], _val[1] - r._val[1], _val[2] - r._val[2]); }
            inline Float3 operator/(const float &r) { return Float3(_val[0] / r, _val[1] / r, _val[2] / r); }
            inline Float3 operator*(const float &r) const { return Float3(r * _val[0], r * _val[1], r * _val[2]); }

            inline float &operator[](const int idx) { return _val[idx]; }
            inline float operator[](const int idx) const { return _val[idx]; }

            //dot product
            inline float operator*(const Float3 &r) const { return _val[0] * r._val[0] + _val[1] * r._val[1] + _val[2] * r._val[2]; }

            //cross product
            inline Float3 operator^(const Float3 &r) const { return Float3(_val[1] * r._val[2] - _val[2] * r._val[1],
                                                                           _val[2] * r._val[0] - _val[0] * r._val[2],
                                                                           _val[0] * r._val[1] - _val[1] * r._val[0]); }


            inline float x() const { return _val[0]; }
            inline float y() const { return _val[1]; }
            inline float z() const { return _val[2]; }

            //swizzle
            inline Float3 xzy() const { return Float3(_val[0], _val[2], _val[1]); }
            inline Float3 yxz() const { return Float3(_val[1], _val[0], _val[2]); }
            inline Float3 yzx() const { return Float3(_val[1], _val[2], _val[0]); }
            inline Float3 zxy() const { return Float3(_val[2], _val[0], _val[1]); }
            inline Float3 zyx() const { return Float3(_val[2], _val[1], _val[0]); }

            //length / normalization
            inline void normalize() { float m = magnitude(); _val[0] /= m; _val[1] /= m; _val[2] /= m; }
            inline float magnitude() const { return sqrt(_val[0] * _val[0] + _val[1] * _val[1] + _val[2] * _val[2]); }
            inline float mag_squared() const { return _val[0] * _val[0] + _val[1] * _val[1] + _val[2] * _val[2]; }

            //data
            float _val[3];
    };

  inline Float2 operator*(const float &l, const Float2 &r) { return Float2(l * r[0], l * r[1]); }
  inline Float3 operator*(const float &l, const Float3 &r) { return Float3(l * r[0], l * r[1], l * r[2]); }
  inline std::ostream &operator<<(std::ostream &os, const Float3 &obj) { os<<"("<<obj[0]<<", "<<obj[1]<<", "<<obj[2]<<")"; return os; }
  inline std::ostream &operator<<(std::ostream &os, const Float2 &obj) { os<<"("<<obj[0]<<", "<<obj[1]<<")"; return os; }

  inline Float2 midpoint(Float2 &a, Float2 &b) { return a + 0.5f * (b - a); }
  inline Float3 midpoint(Float3 &a, Float3 &b) { return a + 0.5f * (b - a); }

  inline float distance(Float2 &a, Float2 &b) { Float2 v = b - a; return v.magnitude(); }
  inline float dist_squared(Float2 &a, Float2 &b) { Float2 v = b - a; return v.mag_squared(); }
  inline float distance(Float3 &a, Float3 &b) { Float3 v = b - a; return v.magnitude(); }
  inline float dist_squared(Float3 &a, Float3 &b) { Float3 v = b - a; return v.mag_squared(); }

	inline float lerp(float x, float y, float m) { return (x * (1.0f - m) + y * m); }
	inline float cerp(float x, float y, float m) { double mu2 = (1.0f - cos(m * M_PI)) / 2.0f; return (x * (1.0f - mu2) + y * mu2); }

	inline Float2 lerp(Float2 &x, Float2 &y, float m) { return Float2(lerp(x._val[0], y._val[0], m), lerp(x._val[1], y._val[1], m)); }
  inline Float2 cerp(Float2 &x, Float2 &y, float m) { return Float2(cerp(x._val[0], y._val[0], m), cerp(x._val[1], y._val[1], m)); }

  inline Float3 lerp(Float3 &x, Float3 &y, float m) { return Float3(lerp(x._val[0], y._val[0], m), lerp(x._val[1], y._val[1], m), lerp(x._val[2], y._val[2], m)); }
  inline Float3 cerp(Float3 &x, Float3 &y, float m) { return Float3(cerp(x._val[0], y._val[0], m), cerp(x._val[1], y._val[1], m), cerp(x._val[2], y._val[2], m)); }

	float clamp(float x, float a, float b);
	float max(float a, float b);
	float min(float a, float b);

	float random(float a, float b);
	int random(int a, int b);

  inline float degrees_to_radians(float d) { return (d / 180.0f) * M_PI; }
	inline float radians_to_degrees(float r) { return (r / M_PI) * 180.0f; }

	unsigned int hash(unsigned char *str);
}

#endif // _MATH_UTILITY_H_
