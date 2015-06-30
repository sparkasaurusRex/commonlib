#ifndef __FLUID2D_INTERACTOR_H__
#define __FLUID2D_INTERACTOR_H__

#include "math_utility.h"

using namespace Math;

class Fluid2DInteractor
{
public:
  Fluid2DInteractor();
  ~Fluid2DInteractor();

  void set_pos(const Float2 p);
  Float2 get_pos() const;
protected:
  Float2 pos;
};

#endif //__FLUID2D_INTERACTOR_H__
