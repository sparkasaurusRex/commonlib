#include <GL/glew.h>
#include <iostream>

#include "material.h"

using namespace std;

Material::Material()
{
    lighting = false;
    transparent = false;

    //standard alpha blending
    src_blend_param = GL_SRC_COLOR;
    dst_blend_param = GL_ONE_MINUS_SRC_ALPHA;

    alpha = 1.0f;

    diff_rgb = Float3(0.5f, 0.5f, 0.5f);
    spec_rgb = Float3(0.8f, 0.7f, 0.2f);
    emit_rgb = Float3(0.0f, 0.0f, 0.0f);

    shader = new Shader;
}

Material::~Material()
{
    delete shader;
}

void Material::init()
{
    shader->load_link_and_compile();
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

void Material::set_alpha(const float a)
{
    alpha = a;
}

void Material::set_diff_rgb(const Float3 d)
{
    diff_rgb = d;
}

void Material::set_spec_rgb(const Float3 s)
{
    spec_rgb = s;
}

void Material::set_emit_rgb(const Float3 e)
{
    emit_rgb = e;
}

void Material::add_tex_idx(const int tid)
{
    texture_ids.push_back(tid);
}

void Material::render_gl() const
{
    shader->render_gl();

    //set up uniforms
    GLint diff_loc = glGetUniformLocationARB(shader->gl_shader_program, "diff_rgb");
    glUniform3fARB(diff_loc, diff_rgb[0], diff_rgb[1], diff_rgb[2]);

    GLint amb_loc = glGetUniformLocationARB(shader->gl_shader_program, "amb_rgb");
    glUniform3fARB(amb_loc, 0.2f, 0.5f, 0.1f);

    //set up textures
    for(int i = 0; i < texture_ids.size(); i++)
    {
        //cout<<"tex_id: "<<texture_ids[i]<<endl;
        //glActiveTexture(GL_TEXTURE0 + i);
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

    //set up textures / shaders...
}
