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

  //NOTE: we assume that by this point the shader has been loaded, compiled, linked, etc...

  //create uniforms for all the textures
  for(int i = 0; i < textures_2d.size(); i++)
  {
    ShaderUniformInt sui;
    sui.set_name(textures_2d[i].second);
    sui.set_loc(shader);
    sui.set_var(i);

    texture_uniforms.push_back(sui);
  }

  //collect all the uniform variable locations
  for(int i = 0; i < shader_uniforms.size(); i++)
  {
    shader_uniforms[i]->set_loc(shader);
  }
}

void Material::add_texture(Texture2D *t, string name)
{
  assert(t);
  std::pair<Texture2D *, string> p(t, name);
  textures_2d.push_back(p);
}

void Material::add_texture(Texture3D *t, string name)
{
  //assert(false);
  assert(t);
  std::pair<Texture3D *, string> p(t, name);
  textures_3d.push_back(p);
}

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

void Material::render() const
{
  shader->render();

  //set up shader uniform variables
  for(int i = 0; i < shader_uniforms.size(); i++)
  {
    shader_uniforms[i]->render();
  }

  for(int i = 0; i < texture_uniforms.size(); i++)
  {
    texture_uniforms[i].render();
  }

  //textures
  for(int i = 0; i < textures_2d.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures_2d[i].first->get_tex_id());
  }

  for(int i = 0; i < textures_3d.size(); i++)
  {
    GLuint actual_tex_slot = GL_TEXTURE0 + textures_2d.size() + i;
    glActiveTexture(actual_tex_slot);
    glEnable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, textures_3d[i].first->get_tex_id());
  }

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
}

void Material::cleanup() const
{
  glUseProgramObjectARB(0);

  for(int i = 0; i < textures_2d.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  for(int i = 0; i < textures_3d.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i + textures_2d.size());
    glDisable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, 0);
  }
}
