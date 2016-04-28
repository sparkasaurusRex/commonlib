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
    ovrHmdDesc            hmd_desc;
    ovrSession            ovr_session;
    ovrSizei              window_size;

    ovrTextureSwapChain   eye_tex_chain[2];
    ovrSizei              eye_tex_size[2];
    GLuint                eye_tex[2];
    GLuint                eye_fbo[2];
    GLuint                depth_tex[2];

    GLuint                mirror_fbo;

    double                sensor_sample_time;
    ovrPosef              eye_render_pose[2];
    long long             frame_index;

    void create_eye_texture(const int eye_idx);
    void create_eye_depth_texture(const int eye_idx);
  public:
    VRContext();
    ~VRContext() {}

    void init();
    void deinit();

    void bind(SDLGame *game);
    void retrieve_eye_poses();
    void render_capture(const unsigned int eye);
    void render_release(const unsigned int eye);
    void finalize_render();

    void simulate(const double game_time, const double frame_time) {}
  };
};

#endif //__VR_H__