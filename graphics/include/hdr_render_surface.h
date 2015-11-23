#ifndef __HDR_RENDER_SURFACE_H__
#define __HDR_RENDER_SURFACE_H__

#include "render_surface.h"

class HDRRenderSurface : public RenderSurface
{
public:
  HDRRenderSurface(const int w = 256, const int h = 256);
  ~HDRRenderSurface();

  void set_exposure(const float e) { exposure = e; }
  void set_bloom_threshold(const float b) { bloom_threshold = b; }

  virtual void init();
  virtual void deinit();
  virtual void render();
private:
  float                  exposure;
  float                  bloom_threshold;
};

#endif //__HDR_RENDER_SURFACE_H__
