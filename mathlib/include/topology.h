#ifndef __TOPOLOGY_H__
#define __TOPOLOGY_H__

namespace Math
{
  enum Topology2D
  {
    TOPOLOGY_2D_PLANE,      //infinite 2d plane
    TOPOLOGY_2D_CYLINDER,   //the 2D surface of an infinitely tall cylinder
    TOPOLOGY_2D_TORUS,      //the 2D surface of a torus (donut)
    TOPOLOGY_2D_SPHERE,     //the 2D surface of a sphere
    NUM_2D_TOPOLOGIES
  };
};

#endif
