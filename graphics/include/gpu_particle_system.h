#ifndef __GPU_PARTICLE_SYSTEM_H__
#define __GPU_PARTICLE_SYSTEM_H__

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <vector>

#include "material.h"
#include "texture.h"
#include "render_surface.h"
#include "texture.h"

#define MAX_NUM_ATTRACTORS 5
#define MAX_AGE 100.f


namespace Graphics {

  enum ParticleForceType {

    ATTRACTOR,

    EMPTY
  };

  class ParticleForce {

  public:

    ParticleForce(int fType) : forceType(fType) {}

    int getForceType() {return forceType;}

  private:

    int forceType;

  };

  class Attractor : public ParticleForce {

  public:

    Attractor(Float3 loc, float s) : ParticleForce(ATTRACTOR), location(loc), strength(s) {}

    virtual Float3 getLocation() {return location;}

    virtual float getStrength() {return strength;}

  private:

    Float3 location;

    float strength;

  };


  class GPUParticleSystem
  {

  public:
    GPUParticleSystem();
    ~GPUParticleSystem();

    void init(Float3 * initial_particle_pos, Float3 * initial_particle_vel, Float3 * colors, float * age);
    void deinit();
    void simulate(const float dt);
    void render();

    void set_num_particles(int num) {num_particles = num;}

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

    GLuint get_pos_tex(const int i) { return pos_tex[i]; }
    GLuint get_vel_tex(const int i) { return vel_tex[i]; }

    void add_force(ParticleForce * f) {
      if (f->getForceType() == ATTRACTOR && numAttractors < MAX_NUM_ATTRACTORS) {
        numAttractors++;
        forces.push_back(f);
      }
    }

    void set_emitter_location(Float3 loc) {emitterLocation = loc;}
    void set_particle_lifespan(float l) {particleLifespan = l;}

    void set_sprite_texture(const char * file) {sprite = new Texture(file); sprite->load();}

  private:

    enum ParticleSystemUniforms
    {
      UNIFORM_UPDATEPOS_EMITTER_LOC,
      UNIFORM_UPDATEPOS_CONSTANTS,
      UNIFORM_UPDATEPOS_POS_TEX,
      UNIFORM_UPDATEPOS_VEL_TEX,

      UNIFORM_UPDATEVEL_CONSTANTS,
      UNIFORM_UPDATEVEL_ATTRACTORS,
      UNIFORM_UPDATEVEL_POS_TEX,
      UNIFORM_UPDATEVEL_VEL_TEX,

      UNIFORM_RENDER_POS_TEX,
      UNIFORM_RENDER_LIFESPAN,
      UNIFORM_RENDER_SPRITE,
      UNIFORM_RENDER_START_COLOR,
      UNIFORM_RENDER_END_COLOR,

      NUM_PARTICLE_UNIFORMS
    };

    struct ParticleVert
    {
      float x, y, z;
      float r, g, b;
      float u, v;
      float sprite_u, sprite_v;
    };

    struct FBOParticleVert
    {
      float x, y, z;
      float u, v;
    };

    std::vector<ParticleForce *> forces;

    int numAttractors;

    float billboard_size;

    Float3 startColor;
    Float3 endColor;

    Float3 emitterLocation;

    float particleLifespan;

    void update_velocities(const float dt);
    void update_positions(const float dt);

    int num_particles;

    Material render_mat;
    Material update_pos_mat;
    Material update_vel_mat;

    GLuint pos_fbo[2];
    GLuint pos_tex[2];

    GLuint vel_fbo[2];
    GLuint vel_tex[2];

    Texture * sprite;

    GLuint vbo;
    GLuint ibo;

    ParticleVert * verts;

    unsigned int * indices;

    GLuint fbo_vbo;
    GLuint fbo_ibo;

    GLuint internal_format;

    std::string update_pos_shader_names[2];
    std::string update_vel_shader_names[2];
    std::string render_shader_names[2];

    GLuint uniform_locations[NUM_PARTICLE_UNIFORMS];
  };

  class GPUParticleSim {

  public:

    ~GPUParticleSim() {
      for (int i = 0; i < pSystems.size(); i++) {
        delete pSystems[i];
      }
      pSystems.clear();
    }

    void addParticleSystem(int numParticles, ParticleForce * * forces, int numForces, Float3 emitterLoc, float emitterRadius, Float3 emitterDirection, float emitterRange, float emitterStrength, float emitterDuration, float lifespan, const char * file);

    void simulate(const float dt) {
      for (int i = 0; i < pSystems.size(); i++) {
        pSystems[i]->simulate(dt);
      }
    }

    void render() {
      for (int i = 0; i < pSystems.size(); i++) {
        pSystems[i]->render();
      }
    }

    GLuint get_pos_tex(const int i) { return pSystems[i]->get_pos_tex(1); }
    GLuint get_vel_tex(const int i) { return pSystems[i]->get_vel_tex(1); }

  private:

    std::vector<GPUParticleSystem *> pSystems;
  };

};


#endif //__GPU_PARTICLE_SYSTEM_H__
