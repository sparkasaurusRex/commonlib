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
    ovrSession    ovr_session;
    ovrSizei      window_size;

    GLuint        eye_tex[2];
    GLuint        eye_fbo[2];
    GLuint        depth_tex[2];

    void create_eye_texture(const int eye_idx);
    void create_eye_depth_texture(const int eye_idx);
  public:
    VRContext();
    ~VRContext() {}

    void init();
    void bind(SDLGame *game);
    void deinit();

    void simulate(const double game_time, const double frame_time) {}
  };
};

#endif //__VR_H__