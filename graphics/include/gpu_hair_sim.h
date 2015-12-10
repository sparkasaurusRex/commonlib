#ifndef __GPU_HAIR_SIM_H__
#define __GPU_HAIR_SIM_H__

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "material.h"
#include "render_surface.h"

namespace Graphics {

  struct HairVert
  {
    float x, y, z;
    float r, g, b;
    float u0, v0;       //index into pos tex
  };

  struct FBOVert
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
    void simulate(const float game_time, const float dt);
    void render();

    //only allowed to call these *before* init!!!
    void set_num_hairs(const int h) { num_hairs = h; }
    void set_num_segments(const int s) { num_segments = s; }

    int get_num_hairs() const { return num_hairs; }
    int get_num_segments() const { return num_segments; }
    GLuint get_pos_tex(const int i) { return pos_tex[i]; }
    GLuint get_force_tex() { return force_tex; }
  private:
    int           num_hairs;
    int           num_segments;
    float         hair_height;

    //texture names where we store hair data (prev & curr)
    GLuint        pos_fbo[2];
    GLuint        pos_tex[2];
    //RenderSurface pos[2];
    GLuint        force_tex;
    int           force_tex_dim[2];

    Material      render_mat;
    Material      sim_mat;

    //vertex and index buffers for the geo
    GLuint        vbo;
    GLuint        ibo;
    HairVert      *verts;
    int           num_verts;
    unsigned int  *indices;
    int           num_indices;

    //vertex and index buffers for the FBOs
    GLuint        fbo_vbo;
    GLuint        fbo_ibo;
    FBOVert       fbo_verts[4];
    int           num_fbo_verts;
    unsigned int  fbo_indices[4];
    int           num_fbo_indices;

    GLuint        internal_format;

    float         k;
  };
};

#endif //__GPU_HAIR_SIM_H__
