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
#include "curve.h"

#define MAX_NUM_ATTRACTORS 5
#define MAX_AGE 100.f
#define DATA_TEXTURE_SIZE 10000


namespace Graphics {

  enum ParticleForceType {

    ATTRACTOR,

    EMPTY
  };

  class ParticleForce
  {
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

    void init(Float3 * initial_particle_pos, Float3 * initial_particle_vel, float * age, float * data);
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

    GLuint get_pos_tex(const int i) { return pos_tex[i]; }
    GLuint get_vel_tex(const int i) { return vel_tex[i]; }
    GLuint get_data_tex() { return data_tex; }

    void add_force(ParticleForce *f)
    {
      if(f->getForceType() == ATTRACTOR && numAttractors < MAX_NUM_ATTRACTORS)
      {
        numAttractors++;
        forces.push_back(f);
      }
    }

    void set_curve_values(int color_id, int num_curves) {
      data_tex_height = num_curves + 1;
      color_curve_id = color_id;
    }
    void set_system_values(float eRadius, float eStrength, float eRange, Float3 eDir, Float3 eLoc, int numP, float life, bool loop, const char * sprite_tex_file) {
      emitter_radius = eRadius;
      emitter_strength = eStrength;
      emitter_range = eRange;
      emitterDirection = eDir;
      emitterLocation = eLoc;
      num_particles = numP;
      particleLifespan = life;
      does_loop = loop;
      sprite = new Texture(sprite_tex_file);
      sprite->load();
    }

  private:

    enum ParticleSystemUniforms
    {
      UNIFORM_UPDATEPOS_EMITTER_LOC,
      UNIFORM_UPDATEPOS_POS_TEX,
      UNIFORM_UPDATEPOS_VEL_TEX,
      UNIFORM_UPDATEPOS_DATA_TEX,
      UNIFORM_UPDATEPOS_CONSTANTS,

      UNIFORM_UPDATEVEL_ATTRACTORS,
      UNIFORM_UPDATEVEL_POS_TEX,
      UNIFORM_UPDATEVEL_VEL_TEX,
      UNIFORM_UPDATEVEL_DATA_TEX,
      UNIFORM_UPDATEVEL_EMITTER_DIR,
      UNIFORM_UPDATEVEL_CONSTANTS,

      UNIFORM_RENDER_POS_TEX,
      UNIFORM_RENDER_LIFESPAN,
      UNIFORM_RENDER_SPRITE,
      UNIFORM_RENDER_DATA_TEX,
      UNIFORM_RENDER_COLOR_CURVE_ID,
      UNIFORM_RENDER_DATA_TEX_HEIGHT,

      NUM_PARTICLE_UNIFORMS
    };

    struct ParticleVert
    {
      float x, y, z;
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

    bool does_loop;


    Float3 emitterLocation;
    Float3 emitterDirection;

    float emitter_range, emitter_strength, emitter_radius;
    int color_curve_id;
    int data_tex_height;

    float particleLifespan;

    void update_velocities(const float game_time, const float dt);
    void update_positions(const float game_time, const float dt);

    int num_particles;

    Material render_mat;
    Material update_pos_mat;
    Material update_vel_mat;

    GLuint pos_fbo[2];
    GLuint pos_tex[2];

    GLuint vel_fbo[2];
    GLuint vel_tex[2];

    GLuint data_tex;

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

    GLint uniform_locations[NUM_PARTICLE_UNIFORMS];
  };

  class GPUParticleSim
  {
  public:

    GPUParticleSim();

    ~GPUParticleSim() {
      for (int i = 0; i < pSystems.size(); i++) {
        delete pSystems[i];
      }
      pSystems.clear();
    }

    void addCurve(const char * fileName, const char * handle);
    void addCurveVec4(const char * file_name_r, const char * file_name_g, const char * file_name_b, const char * file_name_a, const char * handle);


    void addParticleSystem(int numParticles, ParticleForce * * forces, int numForces, Float3 emitterLoc, float emitterRadius, Float3 emitterDirection, float emitterRange, float emitterStrength, float emitterDuration, float lifespan, bool loop, const char * color_handle, const char * tex_file);

    void simulate(const float game_time, const float dt) {
      for (int i = 0; i < pSystems.size(); i++) {
        pSystems[i]->simulate(game_time, dt);
      }
    }

    void render() {
      for (int i = 0; i < pSystems.size(); i++) {
        pSystems[i]->render();
      }
    }

    GLuint get_pos_tex(const int i) { return pSystems[i]->get_pos_tex(1); }
    GLuint get_vel_tex(const int i) { return pSystems[i]->get_vel_tex(1); }
    GLuint get_data_tex() { return pSystems[0]->get_data_tex(); }

  private:

    std::vector<GPUParticleSystem *> pSystems;

    std::vector<float> rand_data;

    std::vector<const char *> curve_handles;

  };

};

#endif //__GPU_PARTICLE_SYSTEM_H__
