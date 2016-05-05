#include "spring_controller.h"

using namespace Physics;
using namespace Math;

SpringController1D::SpringController1D()
{
  pos = 0.0f;
  vel = 0.0f;

  k = 0.0f;
  dampening = 0.0f;
  spring_length = 0.0f;

  external_forces = 0.0f;
}


void SpringController1D::simulate(const double game_time, const double frame_time)
{
  float dist = pos;

  float spring_force = -k * (vel / dist) * (dist - spring_length);
  spring_force = spring_force - spring_force * dampening;

  //ignore mass for now (TODO)
  float final_force = (float)frame_time * (external_forces + spring_force);

  //update spring position
  vel += final_force * (float)frame_time;
  pos += vel * (float)frame_time;
}