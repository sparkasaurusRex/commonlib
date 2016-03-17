#ifndef __OBJECT_3D_H__
#define __OBJECT_3D_H__

#include "math_utility.h"
#include "quaternion.h"

namespace Game
{
  class Object3D
  {
  public:
    Object3D() {}
    ~Object3D() {}

    virtual void init() = 0;
    virtual void simulate(const float dt);
    virtual void render() = 0;

  protected:
    Math::Float3       pos;
    Math::Float3       vel;
    Math::Float3       scale;

    Math::Quaternion   rot;
    Math::Quaternion   ang_vel;
  };
};

#endif //__OBJECT_3D_H__
