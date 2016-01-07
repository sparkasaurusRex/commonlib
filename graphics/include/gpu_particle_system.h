#ifndef __GPU_PARTICLE_SYSTEM_H__
#define __GPU_PARTICLE_SYSTEM_H__

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "material.h"
#include "texture.h"
#include "render_surface.h"

namespace Graphics {
  
  enum ParticleSystemUniforms
  {
    UNIFORM_UPDATEPOS_POS_TEX,
    UNIFORM_UPDATEPOS_VEL_TEX,
    
    UNIFORM_UPDATEVEL_POS_TEX,
    UNIFORM_UPDATEVEL_VEL_TEX,
    
    UNIFORM_RENDER_POS_TEX,
    
    NUM_PARTICLE_UNIFORMS
  };
  
  struct ParticleVert
  {
    float x, y, z;
    float r, g, b;
    float u, v;
  };
  
  struct FBOVert
  {
    float x, y, z;
    float u, v;
  };
  
  class GPUParticleSystem
  {
    
  public:
    GPUParticleSystem();
    ~GPUParticleSystem();
    
    void init(Float3 * initial_particle_pos, Float3 * initial_particle_vel, int _num_particles);
    void deinit();
    void simulate(const float game_time, const float dt);
    void render();
    
    void set_render_shader_names(std::string vs, std::string fs) {
      update_pos_shader_names[0] = vs;
      update_pos_shader_names[1] = fs;
    }
    void set_update_pos_shader_names(std::string vs, std::string fs) {
      update_vel_shader_names[0] = vs;
      update_vel_shader_names[1] = fs;
    }
    void set_update_vel_shader_names(std::string vs, std::string fs)  {
      render_shader_names[0] = vs;
      render_shader_names[1] = fs;
    }
    
  private:

    void update_velocities(const float game_time, const float dt);
    void update_positions(const float game_time, const float dt);
    
    int num_particles;
    
    GLuint pos_fbo[2];
    GLuint pos_tex[2];
    
    GLuint vel_fbo[2];
    GLuint vel_tex[2];
    
    Texture * color_tex;
    
    Material render_mat;
    Material update_pos_mat;
    Material update_vel_mat;
    
    GLuint vbo;
    GLuint ibo;
    
    ParticleVert * verts;
    
    GLuint fbo_vbo;
    GLuint fbo_ibo;
    FBOVert fbo_verts[4];
    int num_fbo_verts;
    unsigned int fbo_indices[4];
    unsigned int * indices;
    
    GLuint internal_format;
    
    
    std::string update_pos_shader_names[2];
    std::string update_vel_shader_names[2];
    std::string render_shader_names[2];
    GLuint uniform_locations[NUM_PARTICLE_UNIFORMS];
  };
};


#endif //__GPU_PARTICLE_SYSTEM_H__

