#ifndef _SHADER_H_
#define _SHADER_H_

#if defined (_WIN32)
#include <Windows.h>
#include <GL/glew.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#endif

#include "math_utility.h"

namespace Graphics
{
  class Shader
  {
  public:
    Shader();
    ~Shader();

    void deinit();

    void set_shader_filenames(std::string vs_fname, std::string fs_fname);
    void compile_and_link_from_source(const char *vs, const char *fs);
    GLuint compile_shader_from_source(GLenum shader_type, const char *source);
    void link_shader();
    
    bool load_link_and_compile();
    void render();


    GLuint gl_fragment_shader;
    GLuint gl_vertex_shader;
    GLuint gl_shader_program;

    std::string gl_fragment_shader_fname;
    std::string gl_vertex_shader_fname;
  };

  class ShaderUniformVariable
  {
  public:
    ShaderUniformVariable() { name = "foo"; loc = 0; }
    ~ShaderUniformVariable() {}

    std::string get_name() const { return name; }
    void set_name(std::string n) { name = n; }
    void set_loc(GLuint l) { loc = l; }
    void set_loc(Shader *sp)
    {
      loc = glGetUniformLocation(sp->gl_shader_program, name.c_str());
    }

    virtual void render() const = 0;  //set uniform variables - to be called prior to Shader::render()
  protected:
    std::string    name;
    GLuint         loc;
  };

  class ShaderUniformInt : public ShaderUniformVariable
  {
  public:
    ShaderUniformInt() : ShaderUniformVariable() { var = 0; }
    ~ShaderUniformInt() {}

    virtual void render() const { glUniform1i(loc, var); }

    void set_var(const int v) { var = v; }
    int get_var() const { return var; }
  private:
    int var;
  };

  class ShaderUniformFloat : public ShaderUniformVariable
  {
  public:
    ShaderUniformFloat() : ShaderUniformVariable() { var = 0.0f; }
    ~ShaderUniformFloat() {}

    virtual void render() const { glUniform1f(loc, var); }
    void set_var(const float v) { var = v; }
  private:
    float var;
  };

  class ShaderUniformFloat2 : public ShaderUniformVariable
  {
  public:
    ShaderUniformFloat2() : ShaderUniformVariable() {}
    ~ShaderUniformFloat2() {}

    virtual void render() const { glUniform2f(loc, var._val[0], var._val[1]); }
    void set_var(const Math::Float2 v) { var = v; }
  private:
    Math::Float2 var;
  };

  class ShaderUniformFloat3 : public ShaderUniformVariable
  {
  public:
    ShaderUniformFloat3() : ShaderUniformVariable() {}
    ~ShaderUniformFloat3() {}

    virtual void render() const { glUniform3f(loc, var._val[0], var._val[1], var._val[2]); }
    void set_var(const Math::Float3 v) { var = v; }
  private:
    Math::Float3 var;
  };
};

#endif // _SHADER_H_
