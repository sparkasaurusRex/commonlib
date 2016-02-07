#include <stddef.h>
#include <iostream>
#include <math.h>
#include <assert.h>

#include "gpu_particle_system.h"
#include "math_utility.h"

using namespace std;
using namespace Math;
using namespace Graphics;

GPUParticleSystem::GPUParticleSystem()
{
  
  billboard_size = 0.035;

  startColor = Float3(1.f, 0.5f, 0.f);
  endColor = Float3(1.f, 0.f, 0.f);
  
  num_particles = 0;

  numAttractors = 0;

  pos_fbo[0] = pos_fbo[1] = 0;
  pos_tex[0] = pos_tex[1] = 0;

  vel_fbo[0] = vel_fbo[1] = 0;
  vel_tex[0] = vel_tex[1] = 0;

  vbo = ibo = 0;
  verts = NULL;
  indices = NULL;
  internal_format = GL_RGBA_FLOAT32_ATI;

  fbo_vbo = fbo_ibo = 0;

  sprite = NULL;

  update_pos_shader_names[0] = std::string("data/shaders/particle_update_pos.vs");
  update_pos_shader_names[1] = std::string("data/shaders/particle_update_pos.fs");
  update_vel_shader_names[0] = std::string("data/shaders/particle_update_vel.vs");
  update_vel_shader_names[1] = std::string("data/shaders/particle_update_vel.fs");
  render_shader_names[0] = std::string("data/shaders/particle_render.vs");
  render_shader_names[1] = std::string("data/shaders/particle_render.fs");
}

GPUParticleSystem::~GPUParticleSystem()
{
  deinit();
}

void GPUParticleSystem::init(Float3 * initial_particle_pos, Float3 * initial_particle_vel, Float3 * colors, float * age)
{

  //create textures
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //create two position textures (prev and curr)
  glGenTextures(2, pos_tex);
  glGenFramebuffersEXT(2, pos_fbo);

  //create two velocity textures (prev and curr)
  glGenTextures(2, vel_tex);
  glGenFramebuffersEXT(2, vel_fbo);

  for (int i = 0; i < 2; i++)
  {
    /* Position Texture */
    GLuint pixel_mode = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, pos_tex[i]);
    assert(glIsTexture(pos_tex[i]) == GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 internal_format,
                 num_particles, //u axis = particle index
                 1,
                 0,
                 pixel_mode,
                 GL_FLOAT,
                 NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    assert(glIsTexture(pos_tex[i]) == GL_TRUE);

    GLfloat * pixels = new GLfloat[num_particles * 4];

    int pixel_idx = 0;

    //Set initial positions into the texture for each particle
    for (int j = 0; j < num_particles; j++)
    {
      pixels[pixel_idx++] = initial_particle_pos[j][0]; //x
      pixels[pixel_idx++] = initial_particle_pos[j][1]; //y
      pixels[pixel_idx++] = initial_particle_pos[j][2]; //z
      pixels[pixel_idx++] = age[j]; //age
    }

    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    0,
                    0,
                    num_particles,
                    1,
                    pixel_mode,
                    GL_FLOAT,
                    pixels);

    delete pixels;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pos_fbo[i]);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, pos_tex[i], 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    /* Velocity Texture */
    glBindTexture(GL_TEXTURE_2D, vel_tex[i]);
    assert(glIsTexture(vel_tex[i]) == GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 internal_format,
                 num_particles, //u axis = particle index
                 1,
                 0,
                 pixel_mode,
                 GL_FLOAT,
                 NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    assert(glIsTexture(vel_tex[i]) == GL_TRUE);

    pixels = new GLfloat[num_particles * 4];

    pixel_idx = 0;

    //Set initial positions into the texture for each particle
    for (int j = 0; j < num_particles; j++)
    {
      pixels[pixel_idx++] = initial_particle_vel[j][0]; //vx
      pixels[pixel_idx++] = initial_particle_vel[j][1]; //vy
      pixels[pixel_idx++] = initial_particle_vel[j][2]; //vz
      pixels[pixel_idx++] = 1.f;                        //Something interesting....
    }

    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    0,
                    0,
                    num_particles,
                    1,
                    pixel_mode,
                    GL_FLOAT,
                    pixels);

    delete pixels;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, vel_fbo[i]);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, vel_tex[i], 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  }

  //Generate Framebuffer Objects

  unsigned int fbo_indices[4] = {0, 1, 2, 3};

  FBOParticleVert fbo_verts[4] = { {-1.f, -1.f, 0.f, 0.f, 0.f},
                                   {1.f, -1.f, 0.f, 1.f, 0.f},
                                   {1.f, 1.f, 0.f, 1.f, 1.f},
                                   {-1.f, 1.f, 0.f, 0.f, 1.f} };

  //Vertex Buffer
  glGenBuffers(1, &fbo_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, fbo_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_verts), fbo_verts, GL_STATIC_DRAW);

  //Index buffer
  glGenBuffers(1, &fbo_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 4, fbo_indices, GL_STATIC_DRAW);


  //Setting up particle data for the render step
  //Mostly we set colors for each particle
  verts = new ParticleVert[num_particles * 4];

  indices = new unsigned int[num_particles * 4];

  float billboard[12] = { -billboard_size, -billboard_size, 0.f,
                          billboard_size, -billboard_size, 0.f,
                          billboard_size, billboard_size, 0.f,
                          -billboard_size, billboard_size, 0.f };

  float uvs[8] = {0.f, 0.f,
                  1.f, 0.f,
                  1.f, 1.f,
                  0.f, 1.f};

  int p_idx = 0;

  for (int v_idx = 0; v_idx < num_particles * 4; v_idx++)
  {
    p_idx = v_idx / 4;

    verts[v_idx].x = billboard[0 + 3 * (v_idx % 4)];
    verts[v_idx].y = billboard[1 + 3 * (v_idx % 4)];
    verts[v_idx].z = billboard[2 + 3 * (v_idx % 4)];
    verts[v_idx].r = colors[p_idx][0];
    verts[v_idx].g = colors[p_idx][1];
    verts[v_idx].b = colors[p_idx][2];
    verts[v_idx].u = (float)p_idx / num_particles; //particle data texture index
    verts[v_idx].v = 0.f;
    verts[v_idx].sprite_u = uvs[0 + 2 * (v_idx % 4)]; //sprite texture uvs
    verts[v_idx].sprite_v = uvs[1 + 2 * (v_idx % 4)];

    indices[v_idx] = v_idx;
  }

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVert) * num_particles * 4, &verts[0].x, GL_STATIC_DRAW);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_particles * 4, indices, GL_STATIC_DRAW);


  //Set up shaders
  render_mat.set_shader_filenames(render_shader_names[0], render_shader_names[1]);
  render_mat.init();

  update_pos_mat.set_shader_filenames(update_pos_shader_names[0], update_pos_shader_names[1]);
  update_pos_mat.init();

  update_vel_mat.set_shader_filenames(update_vel_shader_names[0], update_vel_shader_names[1]);
  update_vel_mat.init();


  //Get shader uniform locations
  update_pos_mat.render_gl();

  Shader * shader = update_pos_mat.get_shader();

  uniform_locations[UNIFORM_UPDATEPOS_POS_TEX] = glGetUniformLocation(shader->gl_shader_program, "prev_pos_tex");
  uniform_locations[UNIFORM_UPDATEPOS_VEL_TEX] = glGetUniformLocation(shader->gl_shader_program, "vel_tex");
  uniform_locations[UNIFORM_UPDATEPOS_CONSTANTS] = glGetUniformLocation(shader->gl_shader_program, "constants");
  uniform_locations[UNIFORM_UPDATEPOS_EMITTER_LOC] = glGetUniformLocation(shader->gl_shader_program, "emitterLocation");


  update_vel_mat.render_gl();
  shader = update_vel_mat.get_shader();

  uniform_locations[UNIFORM_UPDATEVEL_POS_TEX] = glGetUniformLocation(shader->gl_shader_program, "prev_pos_tex");
  uniform_locations[UNIFORM_UPDATEVEL_VEL_TEX] = glGetUniformLocation(shader->gl_shader_program, "vel_tex");
  uniform_locations[UNIFORM_UPDATEVEL_CONSTANTS] = glGetUniformLocation(shader->gl_shader_program, "constants");
  uniform_locations[UNIFORM_UPDATEVEL_ATTRACTORS] = glGetUniformLocation(shader->gl_shader_program, "attractors");


  render_mat.render_gl();
  shader = render_mat.get_shader();

  uniform_locations[UNIFORM_RENDER_POS_TEX] = glGetUniformLocation(shader->gl_shader_program, "particle_tex");
  uniform_locations[UNIFORM_RENDER_LIFESPAN] = glGetUniformLocation(shader->gl_shader_program, "lifespan");
  uniform_locations[UNIFORM_RENDER_SPRITE] = glGetUniformLocation(shader->gl_shader_program, "sprite_tex");
  uniform_locations[UNIFORM_RENDER_START_COLOR] = glGetUniformLocation(shader->gl_shader_program, "startColor");
  uniform_locations[UNIFORM_RENDER_END_COLOR] = glGetUniformLocation(shader->gl_shader_program, "endColor");
  
  glUseProgramObjectARB(0);
}

void GPUParticleSystem::deinit()
{
  glDeleteTextures(2, pos_tex);
  glDeleteTextures(2, vel_tex);

  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  delete verts;
  delete indices;

  verts = NULL;
  indices = NULL;
  pos_tex[0] = pos_tex[1] = vel_tex[0] = vel_tex[1] = vbo = ibo = 0;
  num_particles = 0;

  delete sprite;

  forces.clear();
}

void GPUParticleSystem::update_velocities(const float dt) {

  //Write to  vel_fbo[0]
  //Read from vel_tex[1]
  //Then swap so that vel_tex[1] is always updated

  GLint win_viewport[4];
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, vel_fbo[0]);
  glGetIntegerv(GL_VIEWPORT, win_viewport);
  glViewport(0, 0, num_particles, 1);
  //glClearColor(0.f, 0.f, 0.f, 0.f);
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.f, 1.f, -1.f, 1.f, -10.f, 10.f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  update_vel_mat.render_gl();

  //attractors
  GLfloat * attractors = new GLfloat[4 * MAX_NUM_ATTRACTORS];

  int k = 0;

  for (int i = 0; i < forces.size(); i++) {
    if (forces[i]->getForceType() == ATTRACTOR) {

      Attractor * a = static_cast<Attractor *>(forces[i]);

      Float3 loc = a->getLocation();
      float s = a->getStrength();

      attractors[k++] = loc[0];
      attractors[k++] = loc[1];
      attractors[k++] = loc[2];
      attractors[k++] = s;
    }
  }

  glUniform4fv(uniform_locations[UNIFORM_UPDATEVEL_ATTRACTORS], MAX_NUM_ATTRACTORS, attractors);

  delete attractors;

  //constants
  //{dt, lifespan, numAttractors, ...}
  glUniform4f(uniform_locations[UNIFORM_UPDATEVEL_CONSTANTS], dt, particleLifespan, k / 4, 0);

  //prev_pos_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEVEL_POS_TEX], 0);
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pos_tex[1]);

  //vel_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEVEL_VEL_TEX], 1);
  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, vel_tex[1]);


  glBindBuffer(GL_ARRAY_BUFFER, fbo_vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(FBOParticleVert), (void *)0);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glClientActiveTexture(GL_TEXTURE0);
  glTexCoordPointer(2, GL_FLOAT, sizeof(FBOParticleVert), (void *)(sizeof(float) * 3));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo_ibo);
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (void *)0);

  glUseProgramObjectARB(0);
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glViewport(win_viewport[0], win_viewport[1], win_viewport[2], win_viewport[3]);

  //Swap
  GLuint tmp = vel_tex[0];
  vel_tex[0] = vel_tex[1];
  vel_tex[1] = tmp;

  tmp = vel_fbo[0];
  vel_fbo[0] = vel_fbo[1];
  vel_fbo[1] = tmp;
}

void GPUParticleSystem::update_positions(const float dt) {

  //Write to  pos_fbo[0]
  //Read from pos_tex[1]
  //Then swap so that pos_tex[1] is always updated

  GLint win_viewport[4];
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pos_fbo[0]);
  glGetIntegerv(GL_VIEWPORT, win_viewport);
  glViewport(0, 0, num_particles, 1);
  
  //glClearColor(0.f, 0.f, 0.f, 0.f);
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.f, 1.f, -1.f, 1.f, -10.f, 10.f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  update_pos_mat.render_gl();

  //emitterLocation
  glUniform3f(uniform_locations[UNIFORM_UPDATEPOS_EMITTER_LOC], emitterLocation[0], emitterLocation[1], emitterLocation[2]);

  //constants
  //{dt, lifespan, ..., ...}
  glUniform4f(uniform_locations[UNIFORM_UPDATEPOS_CONSTANTS], dt, particleLifespan, 0, 0);

  //prev_pos_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEVEL_POS_TEX], 0);
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pos_tex[1]);

  //vel_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEVEL_VEL_TEX], 1);
  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, vel_tex[1]);


  glBindBuffer(GL_ARRAY_BUFFER, fbo_vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(FBOParticleVert), (void *)0);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glClientActiveTexture(GL_TEXTURE0);
  glTexCoordPointer(2, GL_FLOAT, sizeof(FBOParticleVert), (void *)(sizeof(float) * 3));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo_ibo);
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (void *)0);

  glUseProgramObjectARB(0);
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glViewport(win_viewport[0], win_viewport[1], win_viewport[2], win_viewport[3]);

  //Swap
  GLuint tmp = pos_tex[0];
  pos_tex[0] = pos_tex[1];
  pos_tex[1] = tmp;

  tmp = pos_fbo[0];
  pos_fbo[0] = pos_fbo[1];
  pos_fbo[1] = tmp;

}

void GPUParticleSystem::simulate(const float dt)
{
  update_velocities(dt);
  update_positions(dt);
}

void GPUParticleSystem::render()
{

  glDisable(GL_CULL_FACE);
  
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  
  render_mat.render_gl();
  
  glUniform1i(uniform_locations[UNIFORM_RENDER_POS_TEX], 0);
  glUniform1i(uniform_locations[UNIFORM_RENDER_SPRITE], 1);
  glUniform1f(uniform_locations[UNIFORM_RENDER_LIFESPAN], particleLifespan);
  glUniform3f(uniform_locations[UNIFORM_RENDER_START_COLOR], startColor[0], startColor[1], startColor[2]);
  glUniform3f(uniform_locations[UNIFORM_RENDER_END_COLOR], endColor[0], endColor[1], endColor[2]);

  
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pos_tex[1]);

  sprite->render_gl(GL_TEXTURE1);
  
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(ParticleVert), (void *)0);

  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(3, GL_FLOAT, sizeof(ParticleVert), (void *)(sizeof(float) * 3));
  
  glClientActiveTexture(GL_TEXTURE0);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(ParticleVert), (void *)(sizeof(float) * 6));
  
  glClientActiveTexture(GL_TEXTURE1);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(ParticleVert), (void *)(sizeof(float) * 8));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_QUADS, num_particles * 4, GL_UNSIGNED_INT, (void *)0);


  glUseProgramObjectARB(0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  
  glClientActiveTexture(GL_TEXTURE1);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glClientActiveTexture(GL_TEXTURE0);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void GPUParticleSim::addParticleSystem(int numParticles, ParticleForce * * forces, int numForces, Float3 emitterLoc, float emitterRadius, Float3 emitterDirection, float emitterRange, float emitterStrength, float emitterDuration, float lifespan, const char * file) {

  GPUParticleSystem * ps = new GPUParticleSystem();

  Float3 * particle_pos = new Float3[numParticles];
  Float3 * particle_vel = new Float3[numParticles];
  Float3 * colors = new Float3[numParticles];
  float * age = new float[numParticles];

  for (int i = 0; i < numParticles; i++) {

    Float3 posOffset = Float3(random(-emitterRadius, emitterRadius), random(-emitterRadius, emitterRadius), random(-emitterRadius, emitterRadius));

    posOffset.normalize();

    posOffset = posOffset * emitterRadius;

    particle_pos[i] = emitterLoc + posOffset * random(0.f, 1.f);

    Float3 velOffset = Float3(random(-emitterRange, emitterRange), random(-emitterRange, emitterRange), random(-emitterRange, emitterRange));

    velOffset.normalize();

    velOffset = velOffset * emitterRange * random(0.f, 1.f);

    particle_vel[i] = emitterDirection + velOffset;

    particle_vel[i].normalize();

    particle_vel[i] = particle_vel[i] * emitterStrength;

    colors[i] = Float3(1.f, 1.f, 0.f);

    age[i] = -emitterDuration * i / numParticles;
  }

  //ps->setEmitterLocation(emitterLoc);
  ps->setParticleLifespan(lifespan);

  for (int i = 0; i < numForces; i++) {
    ps->addForce(forces[i]);
  }

  ps->set_num_particles(numParticles);
  ps->init(particle_pos, particle_vel, colors, age);

  ps->setSpriteTexture(file);

  delete particle_pos;
  delete particle_vel;
  delete colors;
  delete age;

  pSystems.push_back(ps);

}
