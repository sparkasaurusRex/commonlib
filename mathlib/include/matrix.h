#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "math_utility.h"

namespace Math {
  class Matrix2x2
  {
    public:
      Matrix2x2();
      Matrix2x2(const float _00, const float _01, const float _10, const float _11);
      ~Matrix2x2() {};

      Matrix2x2 operator*(const Matrix2x2 &r);
      Float2 operator*(const Float2 &r);

      inline float operator()(const int &row, const int &col) { return m[row][col]; }

      void identity();
      void rotation(float rad);

    private:
      float m[2][2];
  };
}

#endif //__MATRIX_H__
