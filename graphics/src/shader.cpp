#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <GL/glew.h>
#include "shader.h"

//this should definitely not be hard-coded!
#pragma mark HACK
#define SHADER_DIR "data/shaders/"

using namespace std;

Shader::Shader()
{
    gl_fragment_shader = 0;
    gl_vertex_shader = 0;
    gl_shader_program = 0;

    //gl_fragment_source = NULL;
    //gl_vertex_source = NULL;

	strcpy(gl_fragment_shader_fname, SHADER_DIR);
	strcpy(gl_vertex_shader_fname, SHADER_DIR);

    strcat(gl_fragment_shader_fname, "test.fs");
    strcat(gl_vertex_shader_fname, "test.vs");
}

Shader::~Shader()
{
    glDeleteObjectARB(gl_shader_program);
    glDeleteObjectARB(gl_vertex_shader);
    glDeleteObjectARB(gl_fragment_shader);
}

GLuint Shader::load_and_compile_shader(GLenum shader_type, const char *source)
{
    GLuint my_shader = glCreateShaderObjectARB(shader_type);
    glShaderSourceARB(my_shader, 1, &source, NULL);
    glCompileShaderARB(my_shader);

    GLint compiled;
    glGetObjectParameterivARB(my_shader, GL_COMPILE_STATUS, &compiled);
    if (compiled)
    {
        cout<<"shader successfully compiled"<<endl;
    }
    else
    {
        cout<<"shader failed to compile"<<endl;
        GLsizei len;
        GLcharARB log[512];
        glGetInfoLogARB(my_shader, 512, &len, log);
        cout<<log<<endl;
    }
    return my_shader;
}

bool Shader::load_link_and_compile()
{
    GLsizei err_len;
    GLcharARB err_log[512];

    //create the shader program
    gl_shader_program = glCreateProgramObjectARB();
    assert(gl_shader_program);

    //load shader file from disk
    FILE *fp;
    cout<<"loading "<<gl_vertex_shader_fname<<endl;
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
        glAttachObjectARB(gl_shader_program, gl_vertex_shader);

        glGetInfoLogARB(gl_vertex_shader, 512, &err_len, err_log);
        if(strlen(err_log) > 0)
        {
            cerr<<"errors compiling vertex shader!"<<endl;
            cout<<err_log<<endl;
            cout<<gl_vertex_source<<endl;
        }

        free(gl_vertex_source);
        fclose(fp);
    }

    cout<<"loading "<<gl_fragment_shader_fname<<endl;
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
        glAttachObjectARB(gl_shader_program, gl_fragment_shader);

        glGetInfoLogARB(gl_fragment_shader, 512, &err_len, err_log);
        if(strlen(err_log) > 0)
        {
            cerr<<"errors compiling fragment shader!"<<endl;
            cerr<<err_log<<endl;
            cerr<<gl_fragment_source<<endl;
        }

        free(gl_fragment_source);
        fclose(fp);
    }

    glLinkProgramARB(gl_shader_program);

    //check for errors
    glGetInfoLogARB(gl_shader_program, 512, &err_len, err_log);
    if(strlen(err_log) > 0)
    {
        cerr<<"errors linking shader program!"<<endl;
        cerr<<err_log<<endl;
    }

    return true;
}

//apply the shader before rendering
void Shader::render_gl()
{
    glUseProgramObjectARB(gl_shader_program);
}
