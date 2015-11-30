#include "render_surface_combiner.h"

RenderSurfaceCombiner::RenderSurfaceCombiner()
{
 a = b = c = d = NULL;

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
}

RenderSurfaceCombiner::~RenderSurfaceCombiner()
{
  deinit();
}

void RenderSurfaceCombiner::set_surfaces(RenderSurface *_a, RenderSurface *_b, RenderSurface *_c, RenderSurface *_d)
{
  a = _a;
  b = _b;
  c = _c;
  d = _d;
}

void RenderSurfaceCombiner::set_shader_names(std::string vs, std::string fs)
{
  vertex_shader_name = vs;
  fragment_shader_name = fs;
}

void RenderSurfaceCombiner::init()
{
  mat.set_shader_filenames(vertex_shader_name, fragment_shader_name);
  mat.init();

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 4, index_data, GL_STATIC_DRAW);
}

void RenderSurfaceCombiner::deinit()
{

}

void RenderSurfaceCombiner::render()
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

  Shader *shader = mat.get_shader();

  mat.render_gl();


  GLint aloc = glGetUniformLocation(shader->gl_shader_program, "tex_a");
  glUniform1i(aloc, 0);
  GLint bloc = glGetUniformLocation(shader->gl_shader_program, "tex_b");
  glUniform1i(bloc, 1);
  GLint cloc = glGetUniformLocation(shader->gl_shader_program, "tex_c");
  glUniform1i(cloc, 2);
  GLint dloc = glGetUniformLocation(shader->gl_shader_program, "tex_d");
  glUniform1i(dloc, 3);

  glActiveTexture(GL_TEXTURE0);
  glClientActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, a->get_tex());

  glActiveTexture(GL_TEXTURE1);
  glClientActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, b->get_tex());

  glActiveTexture(GL_TEXTURE2);
  glClientActiveTexture(GL_TEXTURE2);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, c->get_tex());

  glActiveTexture(GL_TEXTURE3);
  glClientActiveTexture(GL_TEXTURE3);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, d->get_tex());

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(RenderSurfaceVert), (void *)0);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(RenderSurfaceVert), (void *)(sizeof(float) * 3));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (void *)0);

  //reset shader
  glUseProgramObjectARB(0);
  glActiveTexture(GL_TEXTURE0);
  glClientActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE1);
  glClientActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE2);
  glClientActiveTexture(GL_TEXTURE2);
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE3);
  glClientActiveTexture(GL_TEXTURE3);
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}
