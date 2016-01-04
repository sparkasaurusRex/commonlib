#ifndef __GPU_VORONOI_H__
#define __GPU_VORONOI_H__

#if defined (__APPLE__)
#include <OpenGL/GL.h>
#else
#include <GL/GL.h>
#endif

#include <vector>
#include "math_utility.h"

// 1. collect points (CPU)
// 2. render 2D voronoi diagram in lat / lon (w / wrapping) by
//    rendering a cone for each cell location
// 3. the color of each cone corresponds to a cell location id
// 4. query consists of converting x,y,z to u,v then sampling
//    the voronoi diagram texture

namespace Math
{

  struct ConeVert
  {
    float x, y, z;
    float r, g, b;
  };

  struct FBOVert
  {
    float x, y, z;
    float u, v;
  };

  class GPUVoronoiSphere
  {
    public:
      GPUVoronoiSphere(const GLuint num_seg = 24);
      ~GPUVoronoiSphere();


      void init();
      void deinit();
      void reset();

      void add_point(Math::Float3 pt);

    private:
      std::vector<Math::Float3>     sites;

      GLuint                        num_cone_segments;
      GLuint                        cone_vbo;
      GLuint                        cone_ibo;

      FBOVert                       fbo_verts[4];
      GLuint                        fbo_res[2];
      GLuint                        voronoi_diagram_fbo;
      GLuint                        voronoi_diagram_tex;
  };
};

#endif //__GPU_VORONOI_H__
