#ifndef _SHADER_H_
#define _SHADER_H_

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


namespace Graphics
{
  class Shader
  {
  public:
    Shader();
    ~Shader();

    void set_shader_filenames(std::string vs_fname, std::string fs_fname);
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

  class ShaderUniformVariable
  {
  public:
    ShaderUniformVariable();
    ~ShaderUniformVariable() {}

    void set_loc(GLuint l) { loc = l; }
    void set_loc(Shader *sp)
    {
      glGetUniformLocation(sp->gl_shader_program, name.c_str());
    }

    virtual void render() = 0;  //set uniform variables - to be called prior to Shader::render()
  protected:
    std::string name;
    GLuint loc;
  };

  class ShaderUniformFloat : public ShaderUniformVariable
  {
  public:
    ShaderUniformFloat();
    ~ShaderUniformFloat() {}

    virtual void render() { glUniform1f(loc, var); }
  private:
    float var;
  };
};

#endif // _SHADER_H_
