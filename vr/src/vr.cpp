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


void VRContext::init()
{
  //Initialize OVR system
  ovrResult result = ovr_Initialize(nullptr);
  assert(OVR_SUCCESS(result));

  //create an OVR sesh
  ovrGraphicsLuid luid;
  result = ovr_Create(&session, &luid);
  assert(OVR_SUCCESS(result));

  //poll HMD device (Head Mounted Display)
  hmd_desc = ovr_GetHmdDesc(session);
}

void VRContext::bind(SDLGame *game)
{
  // Setup Window and Graphics
  // Note: the mirror window can be any size, for this sample we use 1/2 the HMD resolution
  ovrSizei window_size = { hmd_desc.Resolution.w / 2, hmd_desc.Resolution.h / 2 };

  // Make eye render buffers
  for (int eye = 0; eye < 2; ++eye)
  {
    ovrSizei idealTextureSize = ovr_GetFovTextureSize(session, ovrEyeType(eye), hmd_desc.DefaultEyeFov[eye], 1);
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
  ovrResult result = ovr_CreateMirrorTextureGL(session, &mtd, &mirror_texture);
}

void VRContext::deinit()
{
  ovr_Shutdown();
}