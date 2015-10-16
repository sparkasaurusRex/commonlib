#include "draw_call.h"

DrawCall::DrawCall()
{
  num_indices = 0;
  vbo = -1;
  ibo = -1;
}

void DrawCall::set_buffer_objects(const GLuint _vbo, const GLuint _ibo)
{
  vbo = _vbo;
  ibo = _ibo;
}
