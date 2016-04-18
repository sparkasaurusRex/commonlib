#ifndef __OBJECT_3D_H__
#define __OBJECT_3D_H__

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#endif

#if defined(_WIN32)
#include <Windows.h>
#include <GL/glew.h>
#endif

#include "math_utility.h"
#include "quaternion.h"

namespace Game
{
  class Object3D
  {
  public:
    Object3D();
    ~Object3D() {}

    virtual void init(const float game_time);
    virtual void simulate(const float gt, const float dt);
    virtual void render();

    void set_pos(const Math::Float3 p) { pos = p; }
    void set_vel(const Math::Float3 v) { vel = v; }
    void set_scale(const Math::Float3 s) { scale = s; }

    float get_age(const float gt) const { return gt - birth_time; }
    float get_birth_time() const { return birth_time; }

    Math::Float3 get_pos() { return pos; }
    Math::Float3 get_vel() { return vel; }
    Math::Float3 get_scale() { return scale; }

  protected:
    Math::Float3       pos;
    Math::Float3       vel;
    Math::Float3       scale;

    float              birth_time;

    //Math::Quaternion   rot;
    //Math::Quaternion   ang_vel;
  };
};

#endif //__OBJECT_3D_H__
