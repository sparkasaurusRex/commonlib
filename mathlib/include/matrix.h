#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "math_utility.h"
#include "quaternion.h"

//
// TODO: use template<int N> to templatize the dimensions of the matrix
//

namespace Math {
  class Matrix2x2
  {
    public:
      Matrix2x2();
      Matrix2x2(const float _00, const float _01, const float _10, const float _11);
      ~Matrix2x2() {};

      Matrix2x2 operator*(const Matrix2x2 &r);
      Float2 operator*(const Float2 &r);

      inline float &operator()(const int &row, const int &col) { return m[row][col]; }
      inline float operator()(const int &row, const int &col) const { return m[row][col]; }

      void identity();
      void rotation(float rad);

    private:
      float m[2][2];
  };

  ostream& operator<<(ostream &os, const Matrix2x2 &m);

  class Matrix3x3
  {
    public:
      Matrix3x3();
      Matrix3x3(const float _00, const float _01, const float _02,
                const float _10, const float _11, const float _12,
                const float _20, const float _21, const float _22);

      ~Matrix3x3() {};

      Matrix3x3 &operator=(const Matrix3x3 &r);

      Float3 operator*(const Float3 &r) const;

      inline float &operator()(const int &row, const int &col) { return m[row][col]; }
      inline float operator()(const int &row, const int &col) const { return m[row][col]; }

      void identity();
      void invert();
      void rotation_from_quaternion(const Quaternion &q);
    private:
      float m[3][3];
  };

  ostream& operator<<(ostream &os, const Matrix3x3 &m);
}

#endif //__MATRIX_H__