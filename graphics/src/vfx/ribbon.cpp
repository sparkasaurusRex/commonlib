#include <iostream>
#include "ribbon.h"
#include "perlin.h"

using namespace Graphics;
using namespace Math;
using namespace std;

Ribbon::Ribbon()
{
  vbo = 0;
  ibo = 0;

  num_verts = 0;
  num_segments = 20;

  root_scale = 0.7f;
  root_speed = 0.0001f;
  tex_scroll_speed = 1.0f;

  profile_noise_bounds[0] = 0.05f;
  profile_noise_bounds[1] = 0.35f;
  profile_noise_scale = 1.2f;
  profile_noise_speed = 0.0003f;

  theta_bounds[0] = 0.0f;//M_PI - 0.1f;
  theta_bounds[1] = 0.5f;//M_PI;
  phi_bounds[0] = 0.0f;
  phi_bounds[1] = 0.5f;

  profile_a.bell_curve_cerp(0.45f, 0.55f);

  mat = NULL;
  tex_a = NULL;
  tex_b = NULL;
}

Ribbon::~Ribbon()
{
  //tear down vbo, ibo
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);

  if(vertex_data) { delete vertex_data; }
  if(index_data) { delete index_data; }
}

void Ribbon::init()
{
  num_verts = num_segments * 2;
  num_indices = num_verts;

  if(vertex_data) { delete vertex_data; }
  if(index_data) { delete index_data; }
  vertex_data = new RibbonVertex[num_verts];
  index_data = new unsigned int[num_indices];

  for(int i = 0; i < num_segments; i++)
  {
    index_data[2 * i] = 2 * i;
    index_data[2 * i + 1] = 2 * i + 1;

    vertex_data[2 * i].x = vertex_data[2 * i + 1].x = 0.0f;
    vertex_data[2 * i].y = vertex_data[2 * i + 1].y = 0.0f;
    vertex_data[2 * i].z = vertex_data[2 * i + 1].z = 0.0f;

    vertex_data[2 * i].nx = vertex_data[2 * i + 1].nx = 1.0f;
    vertex_data[2 * i].ny = vertex_data[2 * i + 1].ny = 0.0f;
    vertex_data[2 * i].nz = vertex_data[2 * i + 1].nz = 0.0f;

    vertex_data[2 * i].r = vertex_data[2 * i + 1].r = 1.0f;
    vertex_data[2 * i].g = vertex_data[2 * i + 1].g = 1.0f;
    vertex_data[2 * i].b = vertex_data[2 * i + 1].b = 1.0f;

    vertex_data[2 * i].u0 = (float)i / (float)(num_segments - 1);
    vertex_data[2 * i].v0 = 0.0f;

    vertex_data[2 * i + 1].u0 = vertex_data[2 * i].u0;
    vertex_data[2 * i + 1].v0 = 1.0f;
  }

  //create vbo / ibo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(RibbonVertex) * num_verts, vertex_data, GL_DYNAMIC_DRAW);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, index_data, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Ribbon::render()
{
  glUseProgramObjectARB(0);
  glDisable(GL_CULL_FACE);

  glEnable(GL_DEPTH_TEST);
  glDepthRange(0.0f, 1.0f);
  glDepthMask(GL_FALSE);

  glPointSize(5.0f);

  Shader *shader = NULL;
  if(mat)
  {
    shader = mat->get_shader();
    mat->render_gl();
  }

  if(tex_a && shader)
  {
    GLuint tex_loc = glGetUniformLocation(shader->gl_shader_program, "noise_1d_tex");
    glUniform1i(tex_loc, 0);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_a->get_tex_id());
    //tex_a->render_gl(0);
  }
  if(tex_b && shader)
  {
    GLuint tex_loc = glGetUniformLocation(shader->gl_shader_program, "grad_vert_tex");
    glUniform1i(tex_loc, 1);
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_b->get_tex_id());
    //tex_b->render_gl(1);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

  //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(RibbonVertex), (void *)0);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(RibbonVertex), (void *)(sizeof(float) * 3));

  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(3, GL_FLOAT, sizeof(RibbonVertex), (void *)(sizeof(float) * 6));
  glClientActiveTexture(GL_TEXTURE0);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(RibbonVertex), (void *)(sizeof(float) * 9));
  //glClientActiveTexture(GL_TEXTURE1);
  //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  //glTexCoordPointer(2, GL_FLOAT, sizeof(AtmosVert), (void *)(sizeof(float) * 11));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_QUAD_STRIP, num_indices, GL_UNSIGNED_INT, (void *)0);
}

void Ribbon::simulate(const float gt, const float dt)
{
  float theta_offset = 0.0f;
  float phi_offset = M_PI;
  float ribbon_length = 0.6f; //in radians?
  //TODO: use dynamic texture and vertex-shader displacement
  for(int i = 0; i < num_segments; i++)
  {
    float seg_pct = (float)i / (float)num_segments;
    float theta = theta_offset + PerlinNoise::scaled_octave_noise_2d(2, 1.0f, root_scale, theta_bounds[0], theta_bounds[1], gt * root_speed, seg_pct);
    float phi = PerlinNoise::scaled_octave_noise_2d(2, 1.0f, root_scale, phi_bounds[0], phi_bounds[1], gt * root_speed + 137.9532f, seg_pct);
    phi = phi_offset + phi + seg_pct * ribbon_length;

    float profile_height = PerlinNoise::scaled_octave_noise_2d(2, 1.0f, profile_noise_scale, profile_noise_bounds[0], profile_noise_bounds[1], gt * profile_noise_speed + 165.132f, seg_pct * profile_noise_scale);
    profile_height *= profile_a.evaluate(seg_pct);

    Float3 root_vert_pos = polar_to_cartesian(theta, phi, 1.1f);
    Float3 top_vert_pos = root_vert_pos + root_vert_pos * profile_height;

    Float3 n = root_vert_pos;
    Float3 tmp = n ^ Float3(0.0f, 1.0f, 0.0f);
    n = n ^ tmp;
    n.normalize();

    int i0 = i * 2;
    int i1 = i0 + 1;

    vertex_data[i0].x = root_vert_pos._val[0];
    vertex_data[i0].y = root_vert_pos._val[1];
    vertex_data[i0].z = root_vert_pos._val[2];

    vertex_data[i0].nx = n._val[0];
    vertex_data[i0].ny = n._val[1];
    vertex_data[i0].nz = n._val[2];

    vertex_data[i1].x = top_vert_pos._val[0];
    vertex_data[i1].y = top_vert_pos._val[1];
    vertex_data[i1].z = top_vert_pos._val[2];

    vertex_data[i1].nx = n._val[0];
    vertex_data[i1].ny = n._val[1];
    vertex_data[i1].nz = n._val[2];
  }

  //update the vbo, and release
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(RibbonVertex) * num_verts, &vertex_data[0].x);
}
