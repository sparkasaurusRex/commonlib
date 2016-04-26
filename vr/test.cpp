#if defined (_WIN32)
#include <Windows.h>
#include <GL/glew.h>
#endif //_WIN32

#include <iostream>

#include "sdl_game.h"
#include "vr.h"

using namespace std;
using namespace VR;

int main(int argc, char **argv)
{

  VRContext vr_context;
  vr_context.init();

  vr_context.deinit();

  return 0;
}