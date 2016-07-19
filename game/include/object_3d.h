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
  protected:
    Math::Float3       pos;
    Math::Float3       vel;
    Math::Float3       scale;

    double             birth_time;

    //Math::Quaternion   rot;
    //Math::Quaternion   ang_vel;

  public:
    Object3D();
    ~Object3D() {}

    virtual void init(const double game_time);
    virtual void simulate(const double game_time, const double frame_time);
    virtual void render(const double game_time);

    void set_pos(const Math::Float3 p) { pos = p; }
    void set_vel(const Math::Float3 v) { vel = v; }
    void set_scale(const Math::Float3 s) { scale = s; }

    double get_age(const double gt) const { return gt - birth_time; }
    double get_birth_time() const { return birth_time; }

    Math::Float3 get_pos() { return pos; }
    Math::Float3 get_vel() { return vel; }
    Math::Float3 get_scale() { return scale; }
  };
};

#endif //__OBJECT_3D_H__
