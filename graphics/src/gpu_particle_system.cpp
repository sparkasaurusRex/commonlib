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
  
  num_particles = 0;
  
  pos_fbo[0] = pos_fbo[1] = 0;
  pos_tex[0] = pos_tex[1] = 0;
  
  vel_fbo[0] = vel_fbo[1] = 0;
  vel_tex[0] = vel_tex[1] = 0;
  
  vbo = ibo = 0;
  verts = NULL;
  indices = NULL;
  color_tex = NULL;
  internal_format = GL_RGBA_FLOAT32_ATI;
  
  //Framebuffer Object data
  fbo_indices[0] = 0;
  fbo_indices[1] = 1;
  fbo_indices[2] = 2;
  fbo_indices[3] = 3;
  
  fbo_verts[0].x =  -1.0f;
  fbo_verts[0].y =  -1.0f;
  fbo_verts[0].z =   0.0f;
  fbo_verts[0].u =   0.0f;
  fbo_verts[0].v =   0.0f;
  
  fbo_verts[1].x =   1.0f;
  fbo_verts[1].y =  -1.0f;
  fbo_verts[1].z =   0.0f;
  fbo_verts[1].u =   1.0f;
  fbo_verts[1].v =   0.0f;
  
  fbo_verts[2].x =   1.0f;
  fbo_verts[2].y =   1.0f;
  fbo_verts[2].z =   0.0f;
  fbo_verts[2].u =   1.0f;
  fbo_verts[2].v =   1.0f;
  
  fbo_verts[3].x =  -1.0f;
  fbo_verts[3].y =   1.0f;
  fbo_verts[3].z =   0.0f;
  fbo_verts[3].u =   0.0f;
  fbo_verts[3].v =   1.0f;
  
  fbo_vbo = fbo_ibo = 0;
  
  
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

void GPUParticleSystem::init(Float3 * initial_particle_pos, Float3 * initial_particle_vel)
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
      pixels[pixel_idx++] = 0.f;                //age
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
      pixels[pixel_idx++] = 0.f;                //Something interesting....
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
  verts = new ParticleVert[num_particles];
  
  indices = new unsigned int[num_particles];
  
  for (int v_idx = 0; v_idx < num_particles; v_idx++)
  {
    verts[v_idx].x = 0.f;
    verts[v_idx].y = 0.f;
    verts[v_idx].z = 0.f;
    verts[v_idx].r = 1.f;
    verts[v_idx].g = 0.f;
    verts[v_idx].b = 0.f;
    verts[v_idx].u = (float)v_idx / num_particles; //particle texture index
    verts[v_idx].v = 0.f;
    
    indices[v_idx] = v_idx;
  }
  
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVert) * num_particles, &verts[0].x, GL_STATIC_DRAW);
  
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_particles, indices, GL_STATIC_DRAW);
  
  
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
  
  
  update_vel_mat.render_gl();
  shader = update_vel_mat.get_shader();
  
  uniform_locations[UNIFORM_UPDATEVEL_POS_TEX] = glGetUniformLocation(shader->gl_shader_program, "prev_pos_tex");
  uniform_locations[UNIFORM_UPDATEVEL_VEL_TEX] = glGetUniformLocation(shader->gl_shader_program, "vel_tex");
  
  
  render_mat.render_gl();
  shader = render_mat.get_shader();
  
  uniform_locations[UNIFORM_RENDER_POS_TEX] = glGetUniformLocation(shader->gl_shader_program, "particle_tex");
  
  glUseProgramObjectARB(0);
}

void GPUParticleSystem::deinit()
{
  glDeleteTextures(2, pos_tex);
  glDeleteTextures(1, vel_tex);
  
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  delete verts;
  delete indices;

  verts = NULL;
  indices = NULL;
  pos_tex[0] = pos_tex[1] = vbo = 0;
  num_particles = 0;
}

void GPUParticleSystem::update_velocities(const float game_time, const float dt) {

  //Write to  vel_fbo[0]
  //Read from vel_tex[1]
  //Then swap so that vel_tex[1] is always updated
  
  GLint win_viewport[4];
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, vel_fbo[0]);
  glGetIntegerv(GL_VIEWPORT, win_viewport);
  glViewport(0, 0, num_particles, 1);
  
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.f, 1.f, -1.f, 1.f, -10.f, 10.f);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  
  update_vel_mat.render_gl();
  Shader * shader = update_vel_mat.get_shader();
  
  //prev_pos_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEVEL_POS_TEX], 0);
  glActiveTexture(GL_TEXTURE0);
  glClientActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pos_tex[1]);
  
  //vel_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEVEL_VEL_TEX], 1);
  glActiveTexture(GL_TEXTURE1);
  glClientActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, vel_tex[1]);
  
  
  glBindBuffer(GL_ARRAY_BUFFER, fbo_vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(FBOParticleVert), (void *)0);
  
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(FBOParticleVert), (void*)(sizeof(float) * 3));
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo_ibo);
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (void *)0);
  
  glUseProgramObjectARB(0);
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE2);
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
  
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.f, 1.f, -1.f, 1.f, -10.f, 10.f);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  
  update_pos_mat.render_gl();
  Shader * shader = update_pos_mat.get_shader();
  
  //prev_pos_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEVEL_POS_TEX], 0);
  glActiveTexture(GL_TEXTURE0);
  glClientActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pos_tex[1]);
  
  //vel_tex
  glUniform1i(uniform_locations[UNIFORM_UPDATEVEL_VEL_TEX], 1);
  glActiveTexture(GL_TEXTURE1);
  glClientActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, vel_tex[1]);
  
  
  glBindBuffer(GL_ARRAY_BUFFER, fbo_vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(FBOParticleVert), (void *)0);
  
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(FBOParticleVert), (void*)(sizeof(float) * 3));
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo_ibo);
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (void *)0);
  
  glUseProgramObjectARB(0);
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE2);
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
 
  render_mat.render_gl();
  Shader * shader = render_mat.get_shader();
  
  glUniform1i(uniform_locations[UNIFORM_RENDER_POS_TEX], 0);
  glActiveTexture(GL_TEXTURE0);
  glClientActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pos_tex[1]);
  
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(ParticleVert), (void *)0);
  
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(3, GL_FLOAT, sizeof(ParticleVert), (void *)(sizeof(float) * 3));
  
  glClientActiveTexture(GL_TEXTURE0);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(ParticleVert), (void *)(sizeof(float) * 6));
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_POINTS, num_particles, GL_FLOAT, (void *) 0);
  
  glUseProgramObjectARB(0);
  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE2);
  glDisable(GL_TEXTURE_2D);
}
