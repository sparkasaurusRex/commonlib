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
  billboard_size = 0.025;
  does_loop = false;

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

void GPUParticleSystem::init(Float3 * initial_particle_pos, Float3 * initial_particle_vel, float * age, float * data)
{
  GLuint pixel_mode = GL_RGBA;

  //create textures
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //create two position textures (prev and curr)
  glGenTextures(2, pos_tex);
  glGenFramebuffersEXT(2, pos_fbo);

  //create two velocity textures (prev and curr)
  glGenTextures(2, vel_tex);
  glGenFramebuffersEXT(2, vel_fbo);

  //create data texture
  glGenTextures(1, &data_tex);

  glBindTexture(GL_TEXTURE_2D, data_tex);
  assert(glIsTexture(data_tex) == GL_TRUE);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               internal_format,
               DATA_TEXTURE_SIZE,
               data_tex_height,
               0,
               pixel_mode,
               GL_FLOAT,
               data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  assert(glIsTexture(data_tex) == GL_TRUE);

  for (int i = 0; i < 2; i++)
  {
    /* Position Texture */
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
                 pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    assert(glIsTexture(pos_tex[i]) == GL_TRUE);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pos_fbo[i]);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, pos_tex[i], 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    /* Velocity Texture */
    pixel_idx = 0;

    //Set initial positions into the texture for each particle
    for (int j = 0; j < num_particles; j++)
    {
      pixels[pixel_idx++] = initial_particle_vel[j][0]; //vx
      pixels[pixel_idx++] = initial_particle_vel[j][1]; //vy
      pixels[pixel_idx++] = initial_particle_vel[j][2]; //vz
      pixels[pixel_idx++] = 1.f;                        //Something interesting....
    }

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
                 pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    assert(glIsTexture(vel_tex[i]) == GL_TRUE);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, vel_fbo[i]);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, vel_tex[i], 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    delete pixels;
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
  uniform_locations[UNIFORM_UPDATEPOS_DATA_TEX] = glGetUniformLocation(shader->gl_shader_program, "data_tex");
  uniform_locations[UNIFORM_UPDATEPOS_EMITTER_LOC] = glGetUniformLocation(shader->gl_shader_program, "emitter_location");
  uniform_locations[UNIFORM_UPDATEPOS_CONSTANTS] = glGetUniformLocation(shader->gl_shader_program, "constants");


  update_vel_mat.render_gl();
  shader = update_vel_mat.get_shader();

  uniform_locations[UNIFORM_UPDATEVEL_POS_TEX] = glGetUniformLocation(shader->gl_shader_program, "prev_pos_tex");
  uniform_locations[UNIFORM_UPDATEVEL_VEL_TEX] = glGetUniformLocation(shader->gl_shader_program, "vel_tex");
  uniform_locations[UNIFORM_UPDATEVEL_DATA_TEX] = glGetUniformLocation(shader->gl_shader_program, "data_tex");
  uniform_locations[UNIFORM_UPDATEVEL_ATTRACTORS] = glGetUniformLocation(shader->gl_shader_program, "attractors");
  uniform_locations[UNIFORM_UPDATEVEL_EMITTER_DIR] = glGetUniformLocation(shader->gl_shader_program, "emitter_direction");
  uniform_locations[UNIFORM_UPDATEVEL_CONSTANTS] = glGetUniformLocation(shader->gl_shader_program, "constants");

  render_mat.render_gl();
  shader = render_mat.get_shader();

  uniform_locations[UNIFORM_RENDER_POS_TEX] = glGetUniformLocation(shader->gl_shader_program, "particle_tex");
  uniform_locations[UNIFORM_RENDER_DATA_TEX] = glGetUniformLocation(shader->gl_shader_program, "data_tex");
  uniform_locations[UNIFORM_RENDER_LIFESPAN] = glGetUniformLocation(shader->gl_shader_program, "lifespan");
  uniform_locations[UNIFORM_RENDER_SPRITE] = glGetUniformLocation(shader->gl_shader_program, "sprite_tex");
  uniform_locations[UNIFORM_RENDER_COLOR_CURVE_ID] = glGetUniformLocation(shader->gl_shader_program, "color_curve_id");
  uniform_locations[UNIFORM_RENDER_DATA_TEX_HEIGHT] = glGetUniformLocation(shader->gl_shader_program, "data_tex_height");

  for (int i = 0; i < NUM_PARTICLE_UNIFORMS; i++) {
    assert(uniform_locations[i] != -1);
  }

  glUseProgramObjectARB(0);
}

void GPUParticleSystem::deinit()
{
  glDeleteTextures(2, pos_tex);
  glDeleteTextures(2, vel_tex);
  glDeleteTextures(1, &data_tex);

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

void GPUParticleSystem::update_velocities(const float game_time, const float dt) {

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
  //{dt, lifespan, num_attractors, does_loop, emitter_range, emitter_strength, game_time}
  const int num_constants = 7;
  GLfloat constants[num_constants] = {dt, particleLifespan, k / 4, does_loop, emitter_range, emitter_strength, game_time};
  glUniform1fv(uniform_locations[UNIFORM_UPDATEVEL_CONSTANTS], num_constants, constants);

  glUniform3f(uniform_locations[UNIFORM_UPDATEVEL_EMITTER_DIR], (GLfloat)emitterDirection[0], (GLfloat)emitterDirection[1], (GLfloat)emitterDirection[2]);

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

  //data_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEVEL_DATA_TEX], 2);
  glActiveTexture(GL_TEXTURE2);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, data_tex);


  glBindBuffer(GL_ARRAY_BUFFER, fbo_vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(FBOParticleVert), (void *)0);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glClientActiveTexture(GL_TEXTURE0);
  glTexCoordPointer(2, GL_FLOAT, sizeof(FBOParticleVert), (void *)(sizeof(float) * 3));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo_ibo);
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (void *)0);

  glUseProgramObjectARB(0);
  glActiveTexture(GL_TEXTURE2);
  glDisable(GL_TEXTURE_2D);
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

void GPUParticleSystem::update_positions(const float game_time, const float dt) {

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
  //{dt, lifespan, does_loop, emitter_radius, game_time}
  const int num_constants = 5;
  GLfloat constants[num_constants] = {dt, particleLifespan, does_loop, emitter_radius, game_time};
  glUniform1fv(uniform_locations[UNIFORM_UPDATEPOS_CONSTANTS], num_constants, constants);

  //prev_pos_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEPOS_POS_TEX], 0);
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pos_tex[1]);

  //vel_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEPOS_VEL_TEX], 1);
  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, vel_tex[1]);

  //data_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEPOS_DATA_TEX], 2);
  glActiveTexture(GL_TEXTURE2);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, data_tex);

  glBindBuffer(GL_ARRAY_BUFFER, fbo_vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(FBOParticleVert), (void *)0);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glClientActiveTexture(GL_TEXTURE0);
  glTexCoordPointer(2, GL_FLOAT, sizeof(FBOParticleVert), (void *)(sizeof(float) * 3));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo_ibo);
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (void *)0);

  glUseProgramObjectARB(0);
  glActiveTexture(GL_TEXTURE2);
  glDisable(GL_TEXTURE_2D);
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

void GPUParticleSystem::simulate(const float game_time, const float dt)
{
  update_velocities(game_time, dt);
  update_positions(game_time, dt);
}

void GPUParticleSystem::render()
{

  glDisable(GL_CULL_FACE);

  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  render_mat.render_gl();

  glUniform1f(uniform_locations[UNIFORM_RENDER_LIFESPAN], particleLifespan);
  glUniform1i(uniform_locations[UNIFORM_RENDER_COLOR_CURVE_ID], color_curve_id);
  glUniform1i(uniform_locations[UNIFORM_RENDER_DATA_TEX_HEIGHT], data_tex_height);

  //pos_tex
  glUniform1i(uniform_locations[UNIFORM_RENDER_POS_TEX], 0);
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pos_tex[1]);

  //sprite_tex
  glUniform1i(uniform_locations[UNIFORM_RENDER_SPRITE], 1);
  sprite->render_gl(GL_TEXTURE1);

  //data_tex
  glUniform1i(uniform_locations[UNIFORM_RENDER_DATA_TEX], 2);
  glActiveTexture(GL_TEXTURE2);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, data_tex);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(ParticleVert), (void *)0);

  glClientActiveTexture(GL_TEXTURE0);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(ParticleVert), (void *)(sizeof(float) * 3));

  glClientActiveTexture(GL_TEXTURE1);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(ParticleVert), (void *)(sizeof(float) * 5));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_QUADS, num_particles * 4, GL_UNSIGNED_INT, (void *)0);

  glUseProgramObjectARB(0);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);

  glDisableClientState(GL_VERTEX_ARRAY);

  glClientActiveTexture(GL_TEXTURE1);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glClientActiveTexture(GL_TEXTURE0);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


GPUParticleSim::GPUParticleSim() {

  // Populate random texture

  for (int i = 0; i < DATA_TEXTURE_SIZE * 4; i++) {
    rand_data.push_back(random(-1.f, 1.f));
  }
}

void GPUParticleSim::addCurveVec4(const char * file_name_r, const char * file_name_g, const char * file_name_b, const char * file_name_a, const char * handle) {

  FILE * file_r = fopen(file_name_r, "rb");
  FILE * file_g = fopen(file_name_g, "rb");
  FILE * file_b = fopen(file_name_b, "rb");
  FILE * file_a = fopen(file_name_a, "rb");

  if (file_r == NULL || file_g == NULL || file_b == NULL || file_a == NULL) {
    cout << "Error! No file named " << file_name_r << ".....or the other ones.\n";
    return;
  }

  Curve curve_r;
  curve_r.fread(file_r);

  Curve curve_g;
  curve_g.fread(file_g);

  Curve curve_b;
  curve_b.fread(file_b);

  Curve curve_a;
  curve_a.fread(file_a);

  curve_handles.push_back(handle);
  float x;
  for (int i = 0; i < DATA_TEXTURE_SIZE; i++) {
    x = (float)i / DATA_TEXTURE_SIZE;
    rand_data.push_back(curve_r.evaluate(x));
    rand_data.push_back(curve_g.evaluate(x));
    rand_data.push_back(curve_b.evaluate(x));
    rand_data.push_back(curve_a.evaluate(x));
  }

  fclose(file_r);
  fclose(file_g);
  fclose(file_b);
  fclose(file_a);

}

void GPUParticleSim::addCurve(const char * file_name, const char * handle) {

  FILE * file = fopen(file_name, "rb");

  if (file_name == NULL) {
    cout << "Error! No file named " << file_name << ".\n";
    return;
  }

  Curve curve;
  curve.fread(file);

  curve_handles.push_back(handle);

  for (int i = 0; i < DATA_TEXTURE_SIZE * 4; i++) {
    rand_data.push_back(curve.evaluate(i / (DATA_TEXTURE_SIZE * 4)));
  }

  fclose(file);

}

void GPUParticleSim::addParticleSystem(int numParticles, ParticleForce * * forces, int numForces, Float3 emitterLoc, float emitterRadius, Float3 emitterDirection, float emitterRange, float emitterStrength, float emitterDuration, float lifespan, bool loop, const char * color_handle, const char * sprite_tex_file) {

  GPUParticleSystem * ps = new GPUParticleSystem();

  Float3 * particle_pos = new Float3[numParticles];
  Float3 * particle_vel = new Float3[numParticles];
  float * age = new float[numParticles];

  for (int i = 0; i < numParticles; i++) {

    Float3 posOffset = Float3(random(-1.f, 1.f), random(-1.f, 1.f), random(-1.f, 1.f));

    posOffset.normalize();

    particle_pos[i] = emitterLoc + emitterRadius * posOffset * random(0.f, 1.f);
    //particle_pos[i] = Float3(0.f, 0.f, 0.f);

    Float3 velOffset = Float3(random(-1.f, 1.f), random(-1.f, 1.f), random(-1.f, 1.f));

    velOffset.normalize();

    velOffset = velOffset * emitterRange * random(-1.f, 1.f);

    particle_vel[i] = emitterDirection + velOffset;

    particle_vel[i].normalize();

    particle_vel[i] = particle_vel[i] * emitterStrength;
    //particle_vel[i] = Float3(0.f, 0.f, 0.f);

    age[i] = -emitterDuration * i / numParticles;
  }

  int color_curve_id = 0;

  for (int i = 0; i < curve_handles.size(); i++) {
    if (curve_handles[i] == color_handle) {
      color_curve_id = i + 1;
    }
  }

  ps->set_curve_values(color_curve_id, curve_handles.size());
  ps->set_system_values(emitterRadius, emitterStrength, emitterRange, emitterDirection, emitterLoc, numParticles, lifespan, loop, sprite_tex_file);

  for (int i = 0; i < numForces; i++) {
    ps->add_force(forces[i]);
  }

  ps->init(particle_pos, particle_vel, age, rand_data.data());


  delete particle_pos;
  delete particle_vel;
  delete age;

  pSystems.push_back(ps);
}
