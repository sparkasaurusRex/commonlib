#ifndef __RENDER_SURFACE_H__
#define __RENDER_SURFACE_H__

#include "material.h"
#include <vector>

struct RenderSurfaceVert
{
  float x, y, z;
  float u, v;
};

class RenderSurface
{
public:
  RenderSurface(const int w = 256, const int h = 256);
  ~RenderSurface();

  void set_fbo_res(const int w, const int h) { fbo_res[0] = w; fbo_res[1] = h; }
  void set_internal_format(GLenum f) { tex_internal_format = f; }
  void set_filtering_mode(GLenum f) { tex_filter = f; }
  void set_shader_names(std::string vs, std::string fs);

  void add_uniform_ptr(Math::Float2 *u, std::string name);
  void add_uniform_tex(GLuint t, std::string name);
  void add_uniform(Math::Float2 &u, std::string name);
  void add_uniform(Math::Float3 &u, std::string name);

  GLuint get_tex() const { return target_tex; }
  Material *get_mat() { return &mat; }
  //void set_mat(Material *m) { mat = m; }

  virtual void init();
  virtual void deinit();
  virtual void render();

  void capture();
  void release();

protected:
  unsigned int           index_data[4];
  RenderSurfaceVert      vertex_data[4];
  GLuint                 vbo;
  GLuint                 ibo;

  GLuint                 target_fbo;
  GLuint                 target_tex;

  bool                   use_depth;
  GLuint                 depth_fbo;

  int                    fbo_res[2];
  GLint                  win_viewport[4];
  GLenum                 tex_internal_format;
  GLenum                 tex_type;
  GLenum                 tex_filter;

  Material               mat;
  std::string            vertex_shader_name;
  std::string            fragment_shader_name;

  //TODO: clean all this uniform BS up
  std::vector<Math::Float2> f2_uni_const;

  std::vector<std::pair<Float2 *, std::string> > uniforms;
  std::vector<std::pair<Float3, std::string> > float3_uniforms;
  std::vector<std::pair<GLuint, std::string> > tex_uniforms;
};

#endif //__RENDER_SURFACE_H__
