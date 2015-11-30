#ifndef _SHADER_H_
#define _SHADER_H_

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/*
class ShaderUniform
{
public:
  ShaderUniform();
  ~ShaderUniform();
protected:
  GLuint loc;
};*/

class Shader
{
public:
  Shader();
  ~Shader();

  GLuint load_and_compile_shader(GLenum shader_type, const char *source);
  bool load_link_and_compile();
  void render_gl();
//private:
  char name[256];

  GLuint gl_fragment_shader;
  GLuint gl_vertex_shader;
  GLuint gl_shader_program;

  char gl_fragment_shader_fname[256];
  char gl_vertex_shader_fname[256];

	//HACK
  int gl_pos_loc;
  int gl_col_loc;
  int gl_uv_loc;
};

#endif // _SHADER_H_
