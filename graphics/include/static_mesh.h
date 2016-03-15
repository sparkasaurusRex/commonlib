#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__

#include <vector>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "math_utility.h"

#include "material.h"
#include "draw_call.h"

namespace Graphics
{
  struct StaticMeshVertex
  {
    float x, y, z;
    float r, g, b;
    float nx, ny, nz;
    float u0, v0;
  };

  class StaticMesh
  {
  public:
    StaticMesh();
    ~StaticMesh();

    void read_from_file(FILE *f);
    void init();
    void render();

  protected:
    std::vector<DrawCall<StaticMeshVertex> > draw_calls;

    int num_vertices;
    StaticMeshVertex *vertices;

    int num_indices;
    unsigned int *indices;

    GLuint vbo;
    GLuint ibo;
  };
};

#endif // __STATIC_MESH_H__
