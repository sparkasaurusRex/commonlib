#include <iostream>
#include <assert.h>

#if defined (_WIN32)
#include "OVR_CAPI_GL.h"
#endif

#include "vr.h"

using namespace VR;
using namespace std;

void VRContext::init()
{
  //Initialize OVR system
  ovrResult result = ovr_Initialize(nullptr);
  assert(OVR_SUCCESS(result));

  //create an OVR sesh
  ovrSession session;
  ovrGraphicsLuid luid;
  result = ovr_Create(&session, &luid);
  assert(OVR_SUCCESS(result));

  //poll HMD device (Head Mounted Display)
  ovrHmdDesc hmd_desc = ovr_GetHmdDesc(session);

  // Setup Window and Graphics
  // Note: the mirror window can be any size, for this sample we use 1/2 the HMD resolution
  ovrSizei window_size = { hmd_desc.Resolution.w / 2, hmd_desc.Resolution.h / 2 };

  // Make eye render buffers
 for(int eye = 0; eye < 2; ++eye)
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

  ovrMirrorTexture mirror_texture = nullptr;
  ovrMirrorTextureDesc mtd;
  memset(&mtd, 0, sizeof(mtd));
  mtd.Width = window_size.w;
  mtd.Height = window_size.h;
  mtd.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;

  // Create mirror texture and an FBO used to copy mirror texture to back buffer
  result = ovr_CreateMirrorTextureGL(session, &mtd, &mirror_texture);

}

void VRContext::deinit()
{
  ovr_Shutdown();
}