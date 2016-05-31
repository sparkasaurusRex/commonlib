#if defined (_USE_OCULUS_SDK) || defined (_USE_OPENVR_SDK)

#include <iostream>
#include <assert.h>

#include "sdl_game.h"
#include "vr.h"
#include "math_utility.h"

#if defined (_USE_OCULUS_SDK)
#include "Extras/OVR_Math.h"
using namespace OVR;
#endif //_USE_OCULUS_SDK

using namespace VR;
using namespace std;

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
  near_clip = 0.1f;
  far_clip = 30.0f;

#if defined (_USE_OCULUS_SDK)
  for (int i = 0; i < 2; i++)
  {
    eye_tex[i] = 0;
    eye_fbo[i] = 0;
    depth_tex[i] = 0;
  }

  frame_index = 0;
#endif //_USE_OCULUS_SDK
}

void VRContext::init()
{
#if defined (_USE_OCULUS_SDK)
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
#endif //_USE_OCULUS_SDK
}

void VRContext::deinit()
{
  if (hmd)
  {
    vr::VR_Shutdown();
    hmd = NULL;
  }
#if defined (_USE_OCULUS_SDK)
  ovr_Shutdown();
#endif //_USE_OCULUS_SDK
}

void VRContext::bind(SDLGame *game)
{
  assert(game);

  game->get_resolution(window_dim[0], window_dim[1]);

  //Loading the SteamVR Runtime
  vr::EVRInitError err = vr::VRInitError_None;
  hmd = vr::VR_Init(&err, vr::VRApplication_Scene);

  if (err != vr::VRInitError_None)
  {
    hmd = NULL;
    char buf[1024];
    sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(err));
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "VR_Init Failed", buf, NULL);
    return;
  }

  assert(hmd);
  
  hmd->GetRecommendedRenderTargetSize(&render_target_dim[0], &render_target_dim[1]);

  //CreateFrameBuffer(render_target_dim[0], render_target_dim[1], leftEyeDesc);
  //CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, rightEyeDesc);
  for (unsigned int eye = 0; eye < 2; eye++)
  {
    glGenFramebuffers(1, &eye_fbo[eye]);
    glBindFramebuffer(GL_FRAMEBUFFER, eye_fbo[eye]);

    glGenRenderbuffers(1, &eye_dbo[eye]);
    glBindRenderbuffer(GL_RENDERBUFFER, eye_dbo[eye]);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, render_target_dim[0], render_target_dim[1]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, eye_dbo[eye]);

    glGenTextures(1, &eye_tex[eye]);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, eye_tex[eye]);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, render_target_dim[0], render_target_dim[1], true);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, eye_tex[eye], 0);

    glGenFramebuffers(1, &eye_resolve_fbo[eye]);
    glBindFramebuffer(GL_FRAMEBUFFER, eye_resolve_fbo[eye]);

    glGenTextures(1, &eye_resolve_tex[eye]);
    glBindTexture(GL_TEXTURE_2D, eye_resolve_tex[eye]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, render_target_dim[0], render_target_dim[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, eye_resolve_tex[eye], 0);

    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
      assert(false);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  init_compositor();

#if defined (_USE_OCULUS_SDK)
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
#endif //_USE_OCULUS_SDK
}

void VRContext::retrieve_eye_poses()
{
#if defined (_USE_OCULUS_SDK)
  //retrieve eye poses to send back to the renderer
  ovrEyeRenderDesc eyeRenderDesc[2];
  eyeRenderDesc[0] = ovr_GetRenderDesc(ovr_session, ovrEye_Left, hmd_desc.DefaultEyeFov[0]);
  eyeRenderDesc[1] = ovr_GetRenderDesc(ovr_session, ovrEye_Right, hmd_desc.DefaultEyeFov[1]);

  ovrVector3f hmd_to_eye_offset[2] = { eyeRenderDesc[0].HmdToEyeOffset, eyeRenderDesc[1].HmdToEyeOffset };

  ovr_GetEyePoses(ovr_session, frame_index, ovrTrue, hmd_to_eye_offset, eye_render_pose, &sensor_sample_time);
#endif //_USE_OCULUS_SDK
}

void VRContext::get_eye_camera(const unsigned int eye, Camera *cam) const
{
  assert(hmd);

  vr::HmdMatrix44_t proj_mat = hmd->GetProjectionMatrix((vr::EVREye)eye, near_clip, far_clip, vr::API_OpenGL);
  vr::HmdMatrix34_t pos_mat = hmd->GetEyeToHeadTransform((vr::EVREye)eye);

  //set the camera world view matrix
  cam->set_pos(Float3(pos_mat.m[0][3], pos_mat.m[1][3], pos_mat.m[2][3]));
  cam->set_up(Float3(pos_mat.m[2][1], pos_mat.m[2][2], pos_mat.m[2][3]));
  cam->set_up(Float3(pos_mat.m[0][1], pos_mat.m[0][2], pos_mat.m[0][3]));

  //set the projection matrix
  GLfloat proj_mat_gl[] = { proj_mat.m[0][0], proj_mat.m[1][0], proj_mat.m[2][0], proj_mat.m[3][0],
                            proj_mat.m[0][1], proj_mat.m[1][1], proj_mat.m[2][1], proj_mat.m[3][1],
                            proj_mat.m[0][2], proj_mat.m[1][2], proj_mat.m[2][2], proj_mat.m[3][2],
                            proj_mat.m[0][3], proj_mat.m[1][3], proj_mat.m[2][3], proj_mat.m[3][3] };
  
  cam->set_projection_matrix(proj_mat_gl);

#if defined (_USE_OCULUS_SDK)
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

  GLfloat proj_mat[] = { proj.M[0][0], proj.M[1][0], proj.M[2][0], proj.M[3][0],
                         proj.M[0][1], proj.M[1][1], proj.M[2][1], proj.M[3][1],
                         proj.M[0][2], proj.M[1][2], proj.M[2][2], proj.M[3][2],
                         proj.M[0][3], proj.M[1][3], proj.M[2][3], proj.M[3][3] };

  cam->set_fov(110.0f);//hmd_desc.DefaultEyeFov[eye]);
  cam->set_pos(Float3(shiftedEyePos.x, shiftedEyePos.y, shiftedEyePos.z));
  cam->set_up(Float3(finalUp.x, finalUp.y, finalUp.z));
  cam->set_lookat(Float3(finalForward.x, finalForward.y, finalForward.z));
  cam->set_projection_matrix(proj_mat);
#endif //_USE_OCULUS_SDK
}

void VRContext::render_capture(const unsigned int eye)
{
  glClearColor(0.15f, 0.15f, 0.18f, 1.0f); // nice background color, but not black
  glEnable(GL_MULTISAMPLE);

  // Left Eye
  glBindFramebuffer(GL_FRAMEBUFFER, eye_fbo[eye]);//leftEyeDesc.m_nRenderFramebufferId);
  glViewport(0, 0, render_target_dim[0], render_target_dim[1]);

#if defined (_USE_OCULUS_SDK)
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
#endif //_USE_OCULUS_SDK
}

void VRContext::render_release(const unsigned int eye)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glDisable(GL_MULTISAMPLE);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, eye_fbo[eye]);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, eye_resolve_fbo[eye]);// leftEyeDesc.m_nResolveFramebufferId);

  glBlitFramebuffer(0, 0, render_target_dim[0], render_target_dim[1], 0, 0, render_target_dim[0], render_target_dim[1],
    GL_COLOR_BUFFER_BIT,
    GL_LINEAR);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  glEnable(GL_MULTISAMPLE);
#if defined (_USE_OCULUS_SDK)
  //unset render surface
  glBindFramebuffer(GL_FRAMEBUFFER, eye_fbo[eye]);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

  //commit the swap chain for rendering
  if (eye_tex_chain[eye])
  {
    ovr_CommitTextureSwapChain(ovr_session, eye_tex_chain[eye]);
  }
#endif //_USE_OCULUS_SDK
}

void VRContext::render_stereo_targets()
{

/*  RenderScene(vr::Eye_Left);
  
  
  


  // Right Eye
  glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
  glViewport(0, 0, render_target_dim[0], render_target_dim[1]);

  RenderScene(vr::Eye_Right);
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glDisable(GL_MULTISAMPLE);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rightEyeDesc.m_nResolveFramebufferId);

  glBlitFramebuffer(0, 0, render_target_dim[0], render_target_dim[1], 0, 0, render_target_dim[0], render_target_dim[1],
    GL_COLOR_BUFFER_BIT,
    GL_LINEAR);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  */
}

//set up the geo for distortion
void VRContext::setup_distortion()
{
  if (!hmd)
    return;

  GLushort m_iLensGridSegmentCountH = 43;
  GLushort m_iLensGridSegmentCountV = 43;

  float w = (float)(1.0 / float(m_iLensGridSegmentCountH - 1));
  float h = (float)(1.0 / float(m_iLensGridSegmentCountV - 1));

  float u, v = 0;

  std::vector<VertexDataLens> vVerts(0);
  VertexDataLens vert;

  //left eye distortion verts
  float Xoffset = -1;
  for (int y = 0; y<m_iLensGridSegmentCountV; y++)
  {
    for (int x = 0; x<m_iLensGridSegmentCountH; x++)
    {
      u = x*w; v = 1 - y*h;
      vert.position = Float2(Xoffset + u, -1 + 2 * y*h);
      
      vr::DistortionCoordinates_t dc0 = hmd->ComputeDistortion(vr::Eye_Left, u, v);

      vert.texCoordRed = Float2(dc0.rfRed[0], 1 - dc0.rfRed[1]);
      vert.texCoordGreen = Float2(dc0.rfGreen[0], 1 - dc0.rfGreen[1]);
      vert.texCoordBlue = Float2(dc0.rfBlue[0], 1 - dc0.rfBlue[1]);

      vVerts.push_back(vert);
    }
  }

  //right eye distortion verts
  Xoffset = 0;
  for (int y = 0; y<m_iLensGridSegmentCountV; y++)
  {
    for (int x = 0; x<m_iLensGridSegmentCountH; x++)
    {
      u = x*w; v = 1 - y*h;
      vert.position = Float2(Xoffset + u, -1 + 2 * y*h);

      vr::DistortionCoordinates_t dc0 = hmd->ComputeDistortion(vr::Eye_Right, u, v);

      vert.texCoordRed = Float2(dc0.rfRed[0], 1 - dc0.rfRed[1]);
      vert.texCoordGreen = Float2(dc0.rfGreen[0], 1 - dc0.rfGreen[1]);
      vert.texCoordBlue = Float2(dc0.rfBlue[0], 1 - dc0.rfBlue[1]);

      vVerts.push_back(vert);
    }
  }

  std::vector<GLushort> vIndices;
  GLushort a, b, c, d;

  GLushort offset = 0;
  for (GLushort y = 0; y<m_iLensGridSegmentCountV - 1; y++)
  {
    for (GLushort x = 0; x<m_iLensGridSegmentCountH - 1; x++)
    {
      a = m_iLensGridSegmentCountH*y + x + offset;
      b = m_iLensGridSegmentCountH*y + x + 1 + offset;
      c = (y + 1)*m_iLensGridSegmentCountH + x + 1 + offset;
      d = (y + 1)*m_iLensGridSegmentCountH + x + offset;
      vIndices.push_back(a);
      vIndices.push_back(b);
      vIndices.push_back(c);

      vIndices.push_back(a);
      vIndices.push_back(c);
      vIndices.push_back(d);
    }
  }

  offset = (m_iLensGridSegmentCountH)*(m_iLensGridSegmentCountV);
  for (GLushort y = 0; y<m_iLensGridSegmentCountV - 1; y++)
  {
    for (GLushort x = 0; x<m_iLensGridSegmentCountH - 1; x++)
    {
      a = m_iLensGridSegmentCountH*y + x + offset;
      b = m_iLensGridSegmentCountH*y + x + 1 + offset;
      c = (y + 1)*m_iLensGridSegmentCountH + x + 1 + offset;
      d = (y + 1)*m_iLensGridSegmentCountH + x + offset;
      vIndices.push_back(a);
      vIndices.push_back(b);
      vIndices.push_back(c);

      vIndices.push_back(a);
      vIndices.push_back(c);
      vIndices.push_back(d);
    }
  }

  num_lens_indices = vIndices.size();

  glGenVertexArrays(1, &lens_vao);
  glBindVertexArray(lens_vao);

  glGenBuffers(1, &lens_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, lens_vbo);
  glBufferData(GL_ARRAY_BUFFER, vVerts.size() * sizeof(VertexDataLens), &vVerts[0], GL_STATIC_DRAW);

  glGenBuffers(1, &lens_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lens_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices.size() * sizeof(GLushort), &vIndices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, position));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, texCoordRed));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, texCoordGreen));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, texCoordBlue));

  glBindVertexArray(0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VRContext::render_distortion()
{
  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, window_dim[0], window_dim[1]);

  glBindVertexArray(lens_vao);
  glUseProgram(lens_shader_id);

  //render left lens (first half of index array )
  glBindTexture(GL_TEXTURE_2D, eye_resolve_tex[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glDrawElements(GL_TRIANGLES, num_lens_indices / 2, GL_UNSIGNED_SHORT, 0);

  //render right lens (second half of index array )
  glBindTexture(GL_TEXTURE_2D, eye_resolve_tex[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glDrawElements(GL_TRIANGLES, num_lens_indices / 2, GL_UNSIGNED_SHORT, (const void *)(num_lens_indices));

  glBindVertexArray(0);
  glUseProgram(0);
}

void VRContext::finalize_render()
{
  // for now as fast as possible
  if (hmd)
  {
    //DrawControllers();
    //render_stereo_targets();
    render_distortion();

    vr::Texture_t left_eye_tex = { (void*)eye_resolve_tex[0], vr::API_OpenGL, vr::ColorSpace_Gamma };
    vr::VRCompositor()->Submit(vr::Eye_Left, &left_eye_tex);
    vr::Texture_t right_eye_tex = { (void*)eye_resolve_tex[1], vr::API_OpenGL, vr::ColorSpace_Gamma };
    vr::VRCompositor()->Submit(vr::Eye_Right, &right_eye_tex);
  }

  if (false)//(m_bVblank && m_bGlFinishHack)
  {
    //$ HACKHACK. From gpuview profiling, it looks like there is a bug where two renders and a present
    // happen right before and after the vsync causing all kinds of jittering issues. This glFinish()
    // appears to clear that up. Temporary fix while I try to get nvidia to investigate this problem.
    // 1/29/2014 mikesart
    glFinish();
  }

  // SwapWindow
  {
  //  SDL_GL_SwapWindow(m_pWindow);
  }

  // Clear
  if(false) {
    // We want to make sure the glFinish waits for the entire present to complete, not just the submission
    // of the command. So, we do a clear here right here so the glFinish will wait fully for the swap.
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  // Flush and wait for swap.
  if (false) //(m_bVblank)
  {
    glFlush();
    glFinish();
  }

  // Spew out the controller and pose count whenever they change.
  /*if (m_iTrackedControllerCount != m_iTrackedControllerCount_Last || m_iValidPoseCount != m_iValidPoseCount_Last)
  {
    m_iValidPoseCount_Last = m_iValidPoseCount;
    m_iTrackedControllerCount_Last = m_iTrackedControllerCount;

    dprintf("PoseCount:%d(%s) Controllers:%d\n", m_iValidPoseCount, m_strPoseClasses.c_str(), m_iTrackedControllerCount);
  }*/

  //UpdateHMDMatrixPose();

#if defined (_USE_OCULUS_SDK)
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
#endif //_USE_OCULUS_SDK
}

void VRContext::create_eye_texture(const int eye_idx)
{
#if defined (_USE_OCULUS_SDK)
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
#endif //_USE_OCULUS_SDK
}

void VRContext::create_eye_depth_texture(const int eye_idx)
{
#if defined (_USE_OCULUS_SDK)
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
#endif //_USE_OCULUS_SDK
}

void VRContext::init_compositor()
{
  vr::EVRInitError peError = vr::VRInitError_None;

  if (!vr::VRCompositor())
  {
    assert(false);
  }
}

void VRContext::simulate(const double game_time, const double frame_time)
{
  assert(hmd);

  // Process SteamVR events
  vr::VREvent_t event;
  while (hmd->PollNextEvent(&event, sizeof(event)))
  {
    /*switch (event.eventType)
    {
    case vr::VREvent_TrackedDeviceActivated:
    {
      SetupRenderModelForTrackedDevice(event.trackedDeviceIndex);
      dprintf("Device %u attached. Setting up render model.\n", event.trackedDeviceIndex);
    }
    break;
    case vr::VREvent_TrackedDeviceDeactivated:
    {
      dprintf("Device %u detached.\n", event.trackedDeviceIndex);
    }
    break;
    case vr::VREvent_TrackedDeviceUpdated:
    {
      dprintf("Device %u updated.\n", event.trackedDeviceIndex);
    }
    break;
    }
    */
  }

  // Process SteamVR controller state
  for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
  {
    vr::VRControllerState_t state;
    if (hmd->GetControllerState(unDevice, &state))
    {
      //m_rbShowTrackedDevice[unDevice] = state.ulButtonPressed == 0;
    }
  }
}

#endif //_USE_OCULUS_SDK || _USE_OPENVR_SDK