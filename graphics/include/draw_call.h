#ifndef __DRAW_CALL_H__
#define __DRAW_CALL_H__

class DrawCall
{
public:
  DrawCall();
  ~DrawCall();
private:
  int num_indices;

  GLuint vbo;
  GLuint nbo;
  GLuint ibo;
};

#endif
