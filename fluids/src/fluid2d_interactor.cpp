#include "fluid2d_interactor.h"

Fluid2DInteractor::Fluid2DInteractor()
{
  pos = Float2(0.5f, 0.5f);
}

Fluid2DInteractor::~Fluid2DInteractor() {}

void Fluid2DInteractor::set_pos(const Float2 p)
{
  pos = p;
}

Float2 Fluid2DInteractor::get_pos() const
{
  return pos;
}
