#ifndef _GL_UTILITY_H
#define _GL_UTILITY_H

#if defined (_WIN32)
#include <Windows.h>
#include <GL/glew.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#endif

void gl_sphere(const float *pos, const float *_dim, const float _radius);
void gl_cube(const float *_pos, const float *_dim);
void gl_cube(const float *_pos, const float *_dim, const float theta_z);

#endif //_GL_UTILITY_H
