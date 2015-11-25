#ifndef __RENDER_SURFACE_COMBINER_H__
#define __RENDER_SURFACE_COMBINER_H__

#include "render_surface.h"

class RenderSurfaceCombiner
{
public:
  RenderSurfaceCombiner();
  ~RenderSurfaceCombiner();

  void set_surfaces(RenderSurface *_a, RenderSurface *_b);

  void init();
  void deinit();
  void render();
private:
  RenderSurface *a, *b;

  //quad data
  unsigned int           index_data[4];
  RenderSurfaceVert      vertex_data[4];
  GLuint                 vbo;
  GLuint                 ibo;

  Material               mat;
};

#endif //__RENDER_SURFACE_COMBINER_H__
