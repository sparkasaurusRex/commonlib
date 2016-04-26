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

  //poll HMD
  ovrHmdDesc hmdDesc = ovr_GetHmdDesc(session);
}

void VRContext::deinit()
{
  ovr_Shutdown();
}