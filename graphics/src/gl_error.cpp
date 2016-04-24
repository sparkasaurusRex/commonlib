#include "gl_error.h"

#include <iostream>
#include <assert.h>

void Graphics::gl_check_error()
{
  GLenum err = glGetError();
  if (err != GL_NO_ERROR)
  {
    switch (err)
    {
    case GL_INVALID_ENUM:
      std::cerr << "GL_INVALID_ENUM:" << std::endl;
      std::cerr << "An unacceptable value is specified for an enumerated argument." << std::endl;
      std::cerr << "The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
      break;

    case GL_INVALID_VALUE:
      std::cerr << "GL_INVALID_VALUE:" << std::endl;
      std::cerr << "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
      break;

    case GL_INVALID_OPERATION:
      std::cerr << "GL_INVALID_OPERATION:" << std::endl;
      std::cerr << "The specified operation is not allowed in the current state." << std::endl;
      std::cerr << "The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
      break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
      std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION: " << std::endl;
      std::cerr << "The framebuffer object is not complete." << std::endl;
      std::cerr << "The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
      break;

    case GL_OUT_OF_MEMORY:
      std::cerr << "GL_OUT_OF_MEMORY:" << std::endl;
      std::cerr << "There is not enough memory left to execute the command." << std::endl;
      std::cerr << "The state of the GL is undefined, except for the state of the error flags, after this error is recorded." << std::endl;
      break;

    case GL_STACK_UNDERFLOW:
      std::cerr << "GL_STACK_UNDERFLOW:" << std::endl;
      std::cerr << "An attempt has been made to perform an operation that would cause an internal stack to underflow." << std::endl;
      break;

    case GL_STACK_OVERFLOW:
      std::cerr << "GL_STACK_OVERFLOW:" << std::endl;
      std::cerr << "An attempt has been made to perform an operation that would cause an internal stack to overflow." << std::endl;
      break;
    }
    assert(false);
  }
}