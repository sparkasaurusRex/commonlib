#ifndef __GPU_HAIR_SIM_H__
#define __GPU_HAIR_SIM_H__

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace Graphics {

  struct HairVert
  {
    float x, y, z;
    float u, v;
  };

  class GPUHairSim
  {
  public:
    GPUHairSim();
    ~GPUHairSim();

    void init();
    void deinit();
    void simulate(const float dt);
    void render();

    //only allowed to call these *before* init!!!
    void set_num_hairs(const int h) { num_hairs = h; }
    void set_num_segments(const int s) { num_segments = s; }

    int get_num_hairs() const { return num_hairs; }
    int get_num_segments() const { return num_segments; }
    GLuint get_pos_tex(const int i) { return pos_tex[i]; }
  private:
    int           num_hairs;
    int           num_segments;

    //texture names where we store hair data (prev & curr)
    GLuint        pos_tex[2];

    //vertex and index buffers for the geo
    GLuint        vbo;
    GLuint        ibo;
    HairVert      *verts;
  };
};

#endif //__GPU_HAIR_SIM_H__
