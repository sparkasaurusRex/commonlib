#include "object_3d.h"

using namespace Game;

void Object3D::simulate(const float dt)
{
  pos = pos + (vel * dt);

  //TODO: angular velocity
}
