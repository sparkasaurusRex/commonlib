#ifndef __GL_ERROR_H__
#define __GL_ERROR_H__

#if defined (_WIN32)
#include <Windows.h>
#include <GL/glew.h>
#endif

#if defined (_GL_DEBUG)
namespace Graphics
{
  void gl_check_error();
}
#else
namespace Graphics {}
#define gl_check_error()
#endif //_GL_DEBUG

#endif
