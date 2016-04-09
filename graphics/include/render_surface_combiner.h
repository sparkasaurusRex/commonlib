#ifndef __RENDER_SURFACE_COMBINER_H__
#define __RENDER_SURFACE_COMBINER_H__

#include "render_surface.h"
#include "texture.h"

namespace Graphics
{
  class RenderSurfaceCombiner
  {
  public:
    RenderSurfaceCombiner();
    ~RenderSurfaceCombiner();

    void set_surfaces(RenderSurface *_a, RenderSurface *_b, RenderSurface *_c, RenderSurface *_d);
    void set_shader_names(std::string vs, std::string fs);

    void set_lut_texture(Texture *lt) { lut = lt; }
    void set_vignette_texture(Texture *vt) { vignette = vt; }

    void init();
    void deinit();
    void render();

    //Graphics::Texture *foo_a, *foo_b;

  private:
    RenderSurface *a, *b, *c, *d;
    std::string vertex_shader_name;
    std::string fragment_shader_name;

    //quad data
    unsigned int           index_data[4];
    RenderSurfaceVert      vertex_data[4];
    GLuint                 vbo;
    GLuint                 ibo;

    Material               mat;
    Texture                *lut;
    Texture                *vignette;
  };
};

#endif //__RENDER_SURFACE_COMBINER_H__
