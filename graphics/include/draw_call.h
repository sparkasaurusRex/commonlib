#ifndef __DRAW_CALL_H__
#define __DRAW_CALL_H__

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "material.h"

class DrawCall
{
public:
  DrawCall();
  ~DrawCall() {}

  void set_num_indices(const int n) { num_indices = n; }
  void set_buffer_objects(const GLuint _vbo, const GLuint _ibo);
  void set_material(Material *m) { mat = m; }

private:
  Material *mat;
  int num_indices;

  GLuint vbo;
  GLuint ibo;
};

#endif
