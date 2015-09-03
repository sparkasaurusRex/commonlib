#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif //__APPLE__

#include "shader.h"

using namespace std;

Shader::Shader()
{
  gl_fragment_shader = 0;
  gl_vertex_shader = 0;
  gl_shader_program = 0;

  //gl_fragment_source = NULL;
  //gl_vertex_source = NULL;

  strcat(gl_fragment_shader_fname, "");
  strcat(gl_vertex_shader_fname, "");
}

Shader::~Shader()
{
  glDeleteObjectARB(gl_shader_program);
  glDeleteObjectARB(gl_vertex_shader);
  glDeleteObjectARB(gl_fragment_shader);
}

GLuint Shader::load_and_compile_shader(GLenum shader_type, const char *source)
{
  //GLuint my_shader = glCreateShaderObjectARB(shader_type);
  GLuint my_shader = glCreateShader(shader_type);
  glShaderSource(my_shader, 1, &source, NULL);
  glCompileShader(my_shader);

  GLint compiled = false;
  glGetShaderiv(my_shader, GL_COMPILE_STATUS, &compiled);
  if(!compiled)
  {
    GLint maxLength = 0;
  	glGetShaderiv(my_shader, GL_INFO_LOG_LENGTH, &maxLength);

  	// The maxLength includes the NULL character
  	GLchar errorLog[maxLength];
  	glGetShaderInfoLog(my_shader, maxLength, &maxLength, &errorLog[0]);
    cout<<errorLog<<endl;

  	glDeleteShader(my_shader);
  }
  return my_shader;
}

void print_log(GLuint obj)
{
	int infologLength = 0;
	int maxLength;

	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH, &maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH, &maxLength);

	char infoLog[maxLength];

	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);

	if (infologLength > 0)
		printf("%s\n",infoLog);
}

bool Shader::load_link_and_compile()
{
    GLsizei err_len;
    GLcharARB err_log[512];

    cout<<"loading vertex shader "<<gl_vertex_shader_fname<<endl;

    //create the shader program
    gl_shader_program = glCreateProgram();
    //cout<<"meow"<<endl;
    assert(gl_shader_program);

    //load shader file from disk
    FILE *fp;
    fp = fopen(gl_vertex_shader_fname, "r");
    if(fp)
    {
      fseek(fp, 0, SEEK_END);
      int string_size = ftell(fp);
      rewind(fp);

      //assert(gl_vertex_source == NULL);
      char *gl_vertex_source = (char *)malloc(sizeof(char) * (string_size + 1));
      memset(gl_vertex_source, 0, string_size + 1);
      fread(gl_vertex_source, sizeof(char), string_size, fp);

      gl_vertex_shader = load_and_compile_shader(GL_VERTEX_SHADER_ARB, gl_vertex_source);
      print_log(gl_vertex_shader);

      glAttachShader(gl_shader_program, gl_vertex_shader);
      print_log(gl_shader_program);

      free(gl_vertex_source);
      fclose(fp);
    }
    else
    {
      cout<<"could not load vertex shader file!"<<endl;
    }

    cout<<"loading fragment shader "<<gl_fragment_shader_fname<<endl;
    fp = fopen(gl_fragment_shader_fname, "r");
    if(fp)
    {
      fseek(fp, 0, SEEK_END);
      int string_size = ftell(fp);
      rewind(fp);

      //assert(gl_fragment_source == NULL);
      char *gl_fragment_source = (char *)malloc(sizeof(char) * (string_size + 1));
      memset(gl_fragment_source, 0, string_size + 1);
      fread(gl_fragment_source, sizeof(char), string_size, fp);

      gl_fragment_shader = load_and_compile_shader(GL_FRAGMENT_SHADER_ARB, gl_fragment_source);
      print_log(gl_fragment_shader);

      glAttachShader(gl_shader_program, gl_fragment_shader);
      print_log(gl_shader_program);

      free(gl_fragment_source);
      fclose(fp);
    }

    glLinkProgram(gl_shader_program);
    print_log(gl_shader_program);

    return true;
}

//apply the shader before rendering
void Shader::render_gl()
{
    glUseProgramObjectARB(gl_shader_program);
}
