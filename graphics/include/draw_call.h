#ifndef __DRAW_CALL_H__
#define __DRAW_CALL_H__


#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

class DrawCall
{
public:
  DrawCall();
  ~DrawCall() {}

  void set_num_indices(const int n) { num_indices = n; }

private:
  int num_indices;

  GLuint vbo;
  GLuint ibo;
};

#endif
