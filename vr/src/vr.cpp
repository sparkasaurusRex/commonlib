#include <iostream>
#include <assert.h>

#include "sdl_game.h"
#include "vr.h"

using namespace VR;
using namespace std;

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
  ovrMirrorTexture mirror_texture = nullptr;
  ovrMirrorTextureDesc mtd;
  memset(&mtd, 0, sizeof(mtd));
  mtd.Width = window_size.w;
  mtd.Height = window_size.h;
  mtd.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
  ovrResult result = ovr_CreateMirrorTextureGL(ovr_session, &mtd, &mirror_texture);
}

void VRContext::deinit()
{
  ovr_Shutdown();
}

void VRContext::create_eye_texture(const int eye_idx)
{
  bool render_target = true;

  ovrSizei ideal_tex_size = ovr_GetFovTextureSize(ovr_session, ovrEyeType(eye_idx), hmd_desc.DefaultEyeFov[eye_idx], 1);

  ovrTextureSwapChainDesc desc = {};
  desc.Type = ovrTexture_2D;
  desc.ArraySize = 1;
  desc.Width = ideal_tex_size.w;
  desc.Height = ideal_tex_size.h;
  desc.MipLevels = 1;
  desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
  desc.SampleCount = 1;
  desc.StaticImage = ovrFalse;

  ovrTextureSwapChain tex_chain;
  ovrResult result = ovr_CreateTextureSwapChainGL(ovr_session, &desc, &tex_chain);

  int length = 0;
  ovr_GetTextureSwapChainLength(ovr_session, tex_chain, &length);

  assert(OVR_SUCCESS(result)); //TODO: 
  if(OVR_SUCCESS(result))
  {
    for(int i = 0; i < length; ++i)
    {
      GLuint chain_tex_id;
      ovr_GetTextureSwapChainBufferGL(ovr_session, tex_chain, i, &chain_tex_id);
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

}