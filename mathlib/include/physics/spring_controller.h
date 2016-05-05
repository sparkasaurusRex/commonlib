#ifndef __SPRING_CONTROLLER_H__
#define __SPRING_CONTROLLER_H__

#include "math_utility.h"

namespace Physics
{
  class SpringController1D
  {
  public:
    float pos;
    float vel;

    SpringController1D();
    ~SpringController1D() {}

    void simulate(const double game_time, const double frame_time);
    void add_external_force(const float f) { external_forces += f; }

  protected:
    float k;                 //spring constant
    float dampening;         //dampening constant
    float spring_length;     //ideal spring length
    float mass;              //mass of the weight attached to a spring

    float external_forces;   //accumulated external forces
  };

  class SpringController3D
  {
  public:
    Math::Float3 pos;
    Math::Float3 vel;

    SpringController3D();
    ~SpringController3D() {}

    void simulate(const double game_time, const double frame_time);
    void add_external_force(const Math::Float3 f) { external_forces = external_forces + f; }

  protected:
    float k;                 //spring constant
    float dampening;         //dampening constant
    float spring_length;     //ideal spring length
    float mass;              //mass of the weight attached to a spring

    Math::Float3 external_forces;
  };
};

#endif //__SPRING_CONTROLLER_H__
