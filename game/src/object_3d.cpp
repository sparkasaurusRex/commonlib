

#include <iostream>

#include "object_3d.h"

using namespace std;
using namespace Game;
using namespace Math;

Object3D::Object3D()
{
  scale = Float3(1.0f, 1.0f, 1.0f);
  birth_time = -1;
}

void Object3D::init(const double game_time)
{
  birth_time = game_time;
}

void Object3D::simulate(const double gt, const double dt)
{
  pos = pos + (vel * (float)dt);

  //TODO: angular velocity
}

void Object3D::render(const double game_time)
{
  glTranslatef(pos._val[0], pos._val[1], pos._val[2]);
  glScalef(scale._val[0], scale._val[1], scale._val[2]);
}
