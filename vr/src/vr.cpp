#include <iostream>
#include <assert.h>

#include "sdl_game.h"
#include "vr.h"
#include "math_utility.h"

#include "Extras/OVR_Math.h"

using namespace VR;
using namespace std;
using namespace OVR;
using namespace Graphics;
using namespace Math;

/*
When using the Rift, the left eye sees the left half of the screen, and the right eye sees the right half.
Although varying from person - to - person, human eye pupils are approximately 65 mm apart.
This is known as interpupillary distance(IPD).
The in - application cameras should be configured with the same separation.
*/

VRContext::VRContext()
{
  for (int i = 0; i < 2; i++)
  {
    eye_tex[i] = 0;
    eye_fbo[i] = 0;
    depth_tex[i] = 0;
  }

  frame_index = 0;
}

void VRContext::init()
{
  //Initialize OVR system
  ovrResult result = ovr_Initialize(nullptr);
  assert(OVR_SUCCESS(result));

  //create an OVR sesh
  ovrGraphicsLuid luid;
  result = ovr_Create(&ovr_session, &luid);
  assert(OVR_SUCCESS(result));

  //poll HMD device (Head Mounted Display)
  hmd_desc = ovr_GetHmdDesc(ovr_session);

  frame_index = 0;
}

void VRContext::deinit()
{
  ovr_Shutdown();
}

void VRContext::bind(SDLGame *game)
{
  // Setup Window and Graphics
  // Note: the mirror window can be any size, for this sample we use 1/2 the HMD resolution
  window_size = { hmd_desc.Resolution.w / 2, hmd_desc.Resolution.h / 2 };

  // Make eye render buffers
  for (int eye = 0; eye < 2; ++eye)
  {
    create_eye_texture(eye);
    create_eye_depth_texture(eye);
    /*eyeRenderTexture[eye] = new TextureBuffer(session, true, true, idealTextureSize, 1, NULL, 1);
    eyeDepthBuffer[eye] = new DepthBuffer(eyeRenderTexture[eye]->GetSize(), 0);

    if (!eyeRenderTexture[eye]->TextureChain)
    {
    if (retryCreate) goto Done;
    VALIDATE(false, "Failed to create texture.");
    }*/
  }

  // Create mirror texture and an FBO used to copy mirror texture to back buffer
  ovrMirrorTextureDesc mirror_tex_desc;
  memset(&mirror_tex_desc, 0, sizeof(mirror_tex_desc));
  mirror_tex_desc.Width = window_size.w;
  mirror_tex_desc.Height = window_size.h;
  mirror_tex_desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;

  ovrMirrorTexture mirror_texture = nullptr;
  ovrResult result = ovr_CreateMirrorTextureGL(ovr_session, &mirror_tex_desc, &mirror_texture);
  assert(OVR_SUCCESS(result));

  // Configure the mirror read buffer
  GLuint mirror_tex_id;
  ovr_GetMirrorTextureBufferGL(ovr_session, mirror_texture, &mirror_tex_id);

  glGenFramebuffers(1, &mirror_fbo);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, mirror_fbo);
  glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirror_tex_id, 0);
  glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

  //FloorLevel will give tracking poses where the floor height is 0
  ovr_SetTrackingOriginType(ovr_session, ovrTrackingOrigin_FloorLevel);
}

void VRContext::retrieve_eye_poses()
{
  //retrieve eye poses to send back to the renderer
  ovrEyeRenderDesc eyeRenderDesc[2];
  eyeRenderDesc[0] = ovr_GetRenderDesc(ovr_session, ovrEye_Left, hmd_desc.DefaultEyeFov[0]);
  eyeRenderDesc[1] = ovr_GetRenderDesc(ovr_session, ovrEye_Right, hmd_desc.DefaultEyeFov[1]);

  ovrVector3f hmd_to_eye_offset[2] = { eyeRenderDesc[0].HmdToEyeOffset, eyeRenderDesc[1].HmdToEyeOffset };

  ovr_GetEyePoses(ovr_session, frame_index, ovrTrue, hmd_to_eye_offset, eye_render_pose, &sensor_sample_time);
}

void VRContext::get_eye_camera(const unsigned int eye, Camera *cam) const
{
  // Get view and projection matrices
  static float yaw(3.141592f);
  static Vector3f pos2(0.0f, 0.0f, 0.0f);


  Matrix4f rollPitchYaw = Matrix4f::RotationY(yaw);
  Matrix4f finalRollPitchYaw = rollPitchYaw * Matrix4f(eye_render_pose[eye].Orientation);
  Vector3f finalUp = finalRollPitchYaw.Transform(Vector3f(0, 1, 0));
  Vector3f finalForward = finalRollPitchYaw.Transform(Vector3f(0, 0, -1));
  Vector3f shiftedEyePos = pos2 + rollPitchYaw.Transform(eye_render_pose[eye].Position);

  Matrix4f view = Matrix4f::LookAtRH(shiftedEyePos, shiftedEyePos + finalForward, finalUp);
  Matrix4f proj = ovrMatrix4f_Projection(hmd_desc.DefaultEyeFov[eye], 0.2f, 1000.0f, ovrProjection_None);

  /*
  GLfloat proj_mat[] = { proj.M[0][0], proj.M[0][1], proj.M[0][2], proj.M[0][3],
  proj.M[1][0], proj.M[1][1], proj.M[1][2], proj.M[1][3],
  proj.M[2][0], proj.M[2][1], proj.M[2][2], proj.M[2][3],
  proj.M[3][0], proj.M[3][1], proj.M[3][2], proj.M[3][3] };
  */

  GLfloat proj_mat[] = { proj.M[0][0], proj.M[1][0], proj.M[2][0], proj.M[3][0],
                         proj.M[0][1], proj.M[1][1], proj.M[2][1], proj.M[3][1],
                         proj.M[0][2], proj.M[1][2], proj.M[2][2], proj.M[3][2],
                         proj.M[0][3], proj.M[1][3], proj.M[2][3], proj.M[3][3] };

  cam->set_fov(110.0f);//hmd_desc.DefaultEyeFov[eye]);
  cam->set_pos(Float3(shiftedEyePos.x, shiftedEyePos.y, shiftedEyePos.z));
  cam->set_up(Float3(finalUp.x, finalUp.y, finalUp.z));
  cam->set_lookat(Float3(finalForward.x, finalForward.y, finalForward.z));
  cam->set_projection_matrix(proj_mat);

#if 0

  /*
  GLfloat view_mat[] = { view.M[0][0], view.M[0][1], view.M[0][2], view.M[0][3],
                         view.M[1][0], view.M[1][1], view.M[1][2], view.M[1][3],
                         view.M[2][0], view.M[2][1], view.M[2][2], view.M[2][3],
                         view.M[3][0], view.M[3][1], view.M[3][2], view.M[3][3] };
  */

  GLfloat view_mat[] = { view.M[0][0], view.M[1][0], view.M[2][0], view.M[3][0],
                         view.M[0][1], view.M[1][1], view.M[2][1], view.M[3][1],
                         view.M[0][2], view.M[1][2], view.M[2][2], view.M[3][2],
                         view.M[0][3], view.M[1][3], view.M[2][3], view.M[3][3] };

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(view_mat);



  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(proj_mat);
#endif
}

void VRContext::render_capture(const unsigned int eye)
{
  //set and clear render texture
  GLuint cur_tex_id;
  if (eye_tex_chain[eye])
  {
    int cur_idx;
    ovr_GetTextureSwapChainCurrentIndex(ovr_session, eye_tex_chain[eye], &cur_idx);
    ovr_GetTextureSwapChainBufferGL(ovr_session, eye_tex_chain[eye], cur_idx, &cur_tex_id);
  }
  else
  {
    cur_tex_id = eye_tex[eye];
  }

  glBindFramebuffer(GL_FRAMEBUFFER, eye_fbo[eye]);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cur_tex_id, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex[eye], 0);

  glViewport(0, 0, eye_tex_size[eye].w, eye_tex_size[eye].h);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_FRAMEBUFFER_SRGB);
}

void VRContext::render_release(const unsigned int eye)
{
  //unset render surface
  glBindFramebuffer(GL_FRAMEBUFFER, eye_fbo[eye]);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

  //commit the swap chain for rendering
  if (eye_tex_chain[eye])
  {
    ovr_CommitTextureSwapChain(ovr_session, eye_tex_chain[eye]);
  }
}

void VRContext::finalize_render()
{
  // Do distortion rendering, Present and flush/sync

  ovrLayerEyeFov ld;
  ld.Header.Type = ovrLayerType_EyeFov;
  ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.

  for (int eye = 0; eye < 2; eye++)
  {
    ld.ColorTexture[eye] = eye_tex_chain[eye];// eyeRenderTexture[eye]->TextureChain;
    ld.Viewport[eye] = Recti(eye_tex_size[eye]);
    ld.Fov[eye] = hmd_desc.DefaultEyeFov[eye];
    ld.RenderPose[eye] = eye_render_pose[eye];
    ld.SensorSampleTime = sensor_sample_time;
  }

  ovrLayerHeader* layers = &ld.Header;
  ovrResult result = ovr_SubmitFrame(ovr_session, frame_index, nullptr, &layers, 1);
  assert(OVR_SUCCESS(result));

  //isVisible = (result == ovrSuccess);

  ovrSessionStatus session_status;
  ovr_GetSessionStatus(ovr_session, &session_status);
  if (session_status.ShouldQuit) { assert(false); } //TODO: handle this better
  if (session_status.ShouldRecenter) { ovr_RecenterTrackingOrigin(ovr_session); }

  // Blit mirror texture to back buffer
  glBindFramebuffer(GL_READ_FRAMEBUFFER, mirror_fbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  GLint w = window_size.w;
  GLint h = window_size.h;
  glBlitFramebuffer(0, h, w, 0, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

  frame_index++;
}

void VRContext::create_eye_texture(const int eye_idx)
{
  bool render_target = true;

  eye_tex_size[eye_idx] = ovr_GetFovTextureSize(ovr_session, ovrEyeType(eye_idx), hmd_desc.DefaultEyeFov[eye_idx], 1);

  ovrTextureSwapChainDesc desc = {};
  desc.Type = ovrTexture_2D;
  desc.ArraySize = 1;
  desc.Width = eye_tex_size[eye_idx].w;
  desc.Height = eye_tex_size[eye_idx].h;
  desc.MipLevels = 1;
  desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
  desc.SampleCount = 1;
  desc.StaticImage = ovrFalse;

  ovrResult result = ovr_CreateTextureSwapChainGL(ovr_session, &desc, &eye_tex_chain[eye_idx]);

  int length = 0;
  ovr_GetTextureSwapChainLength(ovr_session, eye_tex_chain[eye_idx], &length);

  assert(OVR_SUCCESS(result)); //TODO: 
  if(OVR_SUCCESS(result))
  {
    for(int i = 0; i < length; ++i)
    {
      GLuint chain_tex_id;
      ovr_GetTextureSwapChainBufferGL(ovr_session, eye_tex_chain[eye_idx], i, &chain_tex_id);
      glBindTexture(GL_TEXTURE_2D, chain_tex_id);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, render_target ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, render_target ? GL_CLAMP_TO_EDGE : GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, render_target ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    }
  }

  /*
  else
  {
    glGenTextures(1, &eye_tex_id[eye_idx]);
    glBindTexture(GL_TEXTURE_2D, eye_tex_id[eye_idx]);

    if (rendertarget)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, texSize.w, texSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  }*/

  /*if(mipLevels > 1)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }*/

  glGenFramebuffers(1, &eye_fbo[eye_idx]);
}

void VRContext::create_eye_depth_texture(const int eye_idx)
{
  ovrSizei ideal_tex_size = ovr_GetFovTextureSize(ovr_session, ovrEyeType(eye_idx), hmd_desc.DefaultEyeFov[eye_idx], 1);

  glGenTextures(1, &depth_tex[eye_idx]);
  glBindTexture(GL_TEXTURE_2D, depth_tex[eye_idx]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  GLenum internal_format = GL_DEPTH_COMPONENT24;
  GLenum type = GL_UNSIGNED_INT;

  /*
  if(GLE_ARB_depth_buffer_float)
  {
    internal_format = GL_DEPTH_COMPONENT32F;
    type = GL_FLOAT;
  }
  */

  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, ideal_tex_size.w, ideal_tex_size.h, 0, GL_DEPTH_COMPONENT, type, NULL);
}