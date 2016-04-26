#ifndef __VR_H__
#define __VR_H__

#if defined (_WIN32)
#include "OVR_CAPI_GL.h"
#endif

#include "sdl_game.h"

namespace VR
{
  class VRContext
  {
  private:
    ovrHmdDesc    hmd_desc;
    ovrSession    session;
  public:
    VRContext() {}
    ~VRContext() {}

    void init();
    void bind(SDLGame *game);
    void deinit();

    void simulate(const double game_time, const double frame_time);
  };
};

#endif //__VR_H__