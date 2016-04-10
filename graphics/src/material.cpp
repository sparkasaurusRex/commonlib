#include <GL/glew.h>
#include <iostream>
#include <assert.h>
#include <cstring>

#include "material.h"

using namespace std;
using namespace Graphics;
using namespace Math;

Material::Material()
{
  lighting = false;
  transparent = false;

  //standard alpha blending
  src_blend_param = GL_SRC_COLOR;
  dst_blend_param = GL_ONE_MINUS_SRC_ALPHA;

  shader = NULL;
}

Material::~Material()
{
}

void Material::init()
{
  //bare minimum, we need a shader
  assert(shader);
  //strcpy(shader->gl_fragment_shader_fname, fs_fname.c_str());
  //strcpy(shader->gl_vertex_shader_fname, vs_fname.c_str());

  //shader->load_link_and_compile();
}

/*
void Material::set_shader_filenames(std::string vs_name, std::string fs_name)
{
  vs_fname = vs_name;
  fs_fname = fs_name;
}*/

void Material::enable_lighting(const bool l)
{
  lighting = l;
}

void Material::enable_blending(const bool b)
{
  transparent = b;
}

void Material::set_blend_mode(const GLenum src, const GLenum dst)
{
  src_blend_param = src;
  dst_blend_param = dst;
}


void Material::render_gl() const
{
  shader->render_gl();

  /*
  //set up uniforms
  GLint diff_loc = glGetUniformLocationARB(shader->gl_shader_program, "diff_rgb");
  glUniform3fARB(diff_loc, diff_rgb[0], diff_rgb[1], diff_rgb[2]);

  GLint amb_loc = glGetUniformLocationARB(shader->gl_shader_program, "amb_rgb");
  glUniform3fARB(amb_loc, 0.2f, 0.5f, 0.1f);
  */


  //set up textures
  /*
  for(int i = 0; i < texture_ids.size(); i++)
  {
    //cout<<"tex_id: "<<texture_ids[i]<<endl;
    //glActiveTexture(GL_TEXTURE0 + i);
  }
  */

  if(lighting)
  {
    glEnable(GL_LIGHTING);
  }
  else
  {
    glDisable(GL_LIGHTING);
  }

  //TODO - we should really render all transparents
  //       at once... with back to front sorting... yeah.
  if(transparent)
  {
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBlendFunc(src_blend_param, dst_blend_param);
  }
  else
  {
    glDisable(GL_BLEND);
  }

  //set up textures / shaders...
}
