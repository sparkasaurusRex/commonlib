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
  num_segments = 10;

  root_scale = 1.0f;
  root_speed = 1.0f;
  tex_scroll_speed = 1.0f;
}

Ribbon::~Ribbon()
{
  if(vertex_data) { delete vertex_data; }
  if(index_data) { delete index_data; }

  //glDeleteBuffers()
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

    vertex_data[2 * i].nx = vertex_data[2 * i + 1].nx = 0.0f;
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
  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  glPointSize(5.0f);

  //cout<<"Ribbon::render()"<<endl;
  //cout<<"num_indices: "<<num_indices<<endl;
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(RibbonVertex), (void *)0);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(RibbonVertex), (void *)(sizeof(float) * 3));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_POINTS, num_indices, GL_UNSIGNED_INT, (void *)0);
}

void Ribbon::simulate(const float gt, const float dt)
{
  float profile_noise_scale = 1.0f;
  float profile_noise_speed = 1.0f;

  //TODO: use dynamic texture and vertex-shader displacement
  for(int i = 0; i < num_segments; i++)
  {
    float seg_pct = (float)i / (float)num_segments;
    float theta = PerlinNoise::scaled_octave_noise_2d(2, 1.0f, root_scale, 0.0f, M_PI, gt * root_speed, seg_pct * root_scale);
    float phi = PerlinNoise::scaled_octave_noise_2d(2, 1.0f, root_scale, -M_PI, M_PI, gt * root_speed + 137.9532f, seg_pct * root_scale);
    float profile_height = PerlinNoise::scaled_octave_noise_2d(2, 1.0f, profile_noise_scale, 0.0f, 1.0f, gt * profile_noise_speed, seg_pct * profile_noise_scale);

    Float3 root_vert_pos = polar_to_cartesian(theta, phi, 1.0f);
    cout<<"root_vert_pos: "<<root_vert_pos<<endl;
    cout<<"profile height: "<<profile_height<<endl;
    vertex_data[i * 2].x = root_vert_pos[0];
    vertex_data[i * 2].y = root_vert_pos[1];
    vertex_data[i * 2].z = root_vert_pos[2];

    vertex_data[i * 2 + 1].x = root_vert_pos[0] + root_vert_pos[0] * profile_height;
    vertex_data[i * 2 + 1].y = root_vert_pos[1] + root_vert_pos[1] * profile_height;
    vertex_data[i * 2 + 1].z = root_vert_pos[2] + root_vert_pos[2] * profile_height;
  }
}
