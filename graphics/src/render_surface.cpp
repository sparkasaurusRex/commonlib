// originally written while on vacation in Vancouver, B.C. with Lila Garcia
// we ate and drank at various restaurants in Gastown and stayed in a condo airBnB.
// Lila nearly got electrocuted when she tried to plug in the coffee grinder,
// and the outlet shot out sparks and melted a spatula. (!!!)
// I cannot describe my excitement the first time I got HDR bloom to work. :)

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <assert.h>
#include "render_surface.h"
#include "texture.h"

using namespace std;
using namespace Graphics;
using namespace Math;

RenderSurface::RenderSurface(const int w, const int h)
{
  index_data[0] = 0;
  index_data[1] = 1;
  index_data[2] = 2;
  index_data[3] = 3;

  vertex_data[0].x =  -1.0f;
  vertex_data[0].y =  -1.0f;
  vertex_data[0].z =   0.0f;
  vertex_data[0].u =   0.0f;
  vertex_data[0].v =   0.0f;

  vertex_data[1].x =   1.0f;
  vertex_data[1].y =  -1.0f;
  vertex_data[1].z =   0.0f;
  vertex_data[1].u =   1.0f;
  vertex_data[1].v =   0.0f;

  vertex_data[2].x =   1.0f;
  vertex_data[2].y =   1.0f;
  vertex_data[2].z =   0.0f;
  vertex_data[2].u =   1.0f;
  vertex_data[2].v =   1.0f;

  vertex_data[3].x =  -1.0f;
  vertex_data[3].y =   1.0f;
  vertex_data[3].z =   0.0f;
  vertex_data[3].u =   0.0f;
  vertex_data[3].v =   1.0f;

  vbo = 0;
  ibo = 0;

  target_fbo = 0;
  target_tex = 0;

  use_depth = true;
  depth_fbo = 0;

  tex_internal_format =  GL_RGB;//GL_RGBA16F_ARB;
  tex_format =           GL_RGB;
  tex_type =             GL_UNSIGNED_BYTE;//GL_HALF_FLOAT_ARB; //GL_FLOAT;
  tex_filter =           GL_LINEAR;

  fbo_res[0] = w;
  fbo_res[1] = h;

  //TEMP (we probably don't want to actually load a new shader for each render surface)
  shader = new Shader;
  target_tex = new Texture2D(fbo_res[0], fbo_res[1]);
}

RenderSurface::~RenderSurface()
{
  deinit();
  delete shader;
  delete target_tex;
}

void RenderSurface::set_shader_names(std::string vs, std::string fs)
{
  vertex_shader_name = vs;
  fragment_shader_name = fs;
}

void RenderSurface::add_uniform_ptr(Float2 *u, std::string name)
{
  std::pair<Float2 *, std::string> uniform_pair(u, name);
  uniforms.push_back(uniform_pair);
}

void RenderSurface::add_uniform(Float2 &v, std::string name)
{
  f2_uni_const.push_back(v);
  Float2 *f2_ptr = &(f2_uni_const[f2_uni_const.size() - 1]);
  std::pair<Float2 *, std::string> f2_pair(f2_ptr, name);
  uniforms.push_back(f2_pair);
  //mat.add_uniform_var();
}

void RenderSurface::add_uniform(Float3 &v, std::string name)
{
  std::pair<Float3, std::string> f3_pair(v, name);
  float3_uniforms.push_back(f3_pair);
}

void RenderSurface::init()
{
  shader->set_shader_filenames(vertex_shader_name, fragment_shader_name);
  shader->load_link_and_compile();
  mat.set_shader(shader);
  mat.set_verbose(true);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 4, index_data, GL_STATIC_DRAW);

  // create a color texture
  //glGenTextures(1, &target_tex);
  //glBindTexture(GL_TEXTURE_2D, target_tex);
  //glTexImage2D(GL_TEXTURE_2D, 0, tex_internal_format, fbo_res[0], fbo_res[1], 0, tex_format, tex_type, 0);
  target_tex->set_tex_format(tex_format);
  target_tex->set_internal_format(tex_internal_format);
  target_tex->set_data_format(tex_type);
  target_tex->set_filtering_mode(tex_filter);
  target_tex->set_wrap_mode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  target_tex->set_resolution(fbo_res[0], fbo_res[1]);
  target_tex->init();

  /*
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex_filter);
  glBindTexture(GL_TEXTURE_2D, 0);*/

  //std::string tex_name("surface_tex");
  //add_uniform_tex(target_tex, tex_name);
  mat.add_texture(target_tex, std::string("surface_tex"));

  mat.init();

  // create depth renderbuffer
  if(use_depth)
  {
    glGenRenderbuffersEXT(1, &depth_fbo);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_fbo);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, fbo_res[0], fbo_res[1]);
  }
  else
  {
    depth_fbo = 0;
  }

  // create FBO
  glGenFramebuffersEXT(1, &target_fbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target_fbo);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, target_tex->get_tex_id(), 0);
  if(use_depth)
  {
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_fbo);
  }

  //undbind the hdr render target (so that we're not rendering to it by default)
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void RenderSurface::deinit()
{

}

void RenderSurface::capture()
{
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target_fbo);
  glGetIntegerv(GL_VIEWPORT, win_viewport);
  glViewport(0, 0, fbo_res[0], fbo_res[1]);
}

void RenderSurface::release()
{
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glViewport(win_viewport[0], win_viewport[1], win_viewport[2], win_viewport[3]);
}

void RenderSurface::render()
{
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  //render the HDR render surface to a full-screen quad
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  mat.render();
  Shader *shader = mat.get_shader();

  for(int i = 0; i < uniforms.size(); i++)
  {
    Float2 *uval = uniforms[i].first;
    std::string uname = uniforms[i].second;
    GLint uloc = glGetUniformLocation(shader->gl_shader_program, uname.c_str());
    glUniform2f(uloc, (*uval)[0], (*uval)[1]);
  }

  for(int i = 0; i < float3_uniforms.size(); i++)
  {
    Float3 uval = float3_uniforms[i].first;
    std::string uname = float3_uniforms[i].second;
    GLint uloc = glGetUniformLocation(shader->gl_shader_program, uname.c_str());
    glUniform3f(uloc, uval[0], uval[1], uval[2]);
  }

/*
  for(int i = 0; i < tex_uniforms.size(); i++)
  {
    GLuint tex_id = tex_uniforms[i].first;
    std::string uname = tex_uniforms[i].second;

    GLint uloc = glGetUniformLocation(shader->gl_shader_program, uname.c_str());
    glUniform1i(uloc, i);

    glActiveTexture(GL_TEXTURE0 + i);
    glClientActiveTexture(GL_TEXTURE0 + i);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_id);
  }
  */


  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(RenderSurfaceVert), (void *)0);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(RenderSurfaceVert), (void *)(sizeof(float) * 3));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (void *)0);

  mat.cleanup();

  //reset shader
  //glUseProgramObjectARB(0);

  /*for(int i = 0; i < tex_uniforms.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glClientActiveTexture(GL_TEXTURE0 + i);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }*/
}
