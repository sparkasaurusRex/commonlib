#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#if defined (__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <vector>

#include "math_utility.h"
#include "shader.h"
#include "texture.h"

namespace Graphics {
  class Material {
    public:
      Material();
      ~Material();

      void init();

      //void set_shader_filenames(std::string vs_name, std::string fs_name);
      void set_shader(Shader *s) { shader = s; }
      Shader *get_shader() { return shader; }

      void enable_lighting(const bool l);
      void enable_blending(const bool b);

      void set_blend_mode(const GLenum src, const GLenum dst);

      void render_gl() const;

      void add_tex_idx(const int tid);

    private:
      Shader                                *shader;
      std::vector<ShaderUniformVariable>    shader_uniforms;
      std::vector<Texture2D *>              textures_2d;
      std::vector<Texture3D *>              textures_3d;

      //lighting parameters
      bool lighting;

      //transparency parameters
      bool transparent;
      GLenum src_blend_param;
      GLenum dst_blend_param;

      //depth parameters
      Math::Float2 depth_range;


      // Math::Float3 diff_rgb;
      // Math::Float3 spec_rgb;
      // Math::Float3 emit_rgb;

      //std::vector<Texture *> textures;
      //std::vector<int> texture_ids;

      //std::string vs_fname;
      //std::string fs_fname;
  };
};

#endif // __MATERIAL_H__
