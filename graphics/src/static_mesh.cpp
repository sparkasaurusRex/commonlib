#include <iostream>

#include <assert.h>
#include <string.h>

//#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "static_mesh.h"

#define RENDER_METHOD_VBO   true

using namespace std;
using namespace Graphics;

StaticMesh::StaticMesh()
{
    vbo = ibo = 0;

    num_vertices = 0;
    vertices = NULL;

    num_indices = 0;
    indices = NULL;
}

StaticMesh::~StaticMesh()
{
  if(vertices)
  {
    delete vertices;
    glDeleteBuffersARB(1, &vbo);
  }

  if(indices)
  {
    glDeleteBuffersARB(1, &ibo);
    delete indices;
  }
}

void StaticMesh::read_from_file(FILE *f)
{
  int version;
  fread(&version, sizeof(int), 1, f);

  //int num_verts, num_indices;
  fread(&num_vertices, sizeof(int), 1, f);
  vertices = new StaticMeshVertex[num_vertices];
  fread(vertices, sizeof(StaticMeshVertex), num_vertices, f);

  cout<<"StaticMesh::read_from_file(): "<<endl;
  cout<<"\t"<<num_vertices<<" vertices..."<<endl;
  for(int i = 0; i < num_vertices; i++)
  {
    cout<<"\t\t"<<vertices[i].x<<", "<<vertices[i].y<<", "<<vertices[i].z<<endl;
    cout<<"\t\t"<<vertices[i].nx<<", "<<vertices[i].ny<<", "<<vertices[i].nz<<endl;
  }

  fread(&num_indices, sizeof(int), 1, f);
  indices = new unsigned int[num_indices];

  cout<<"\t"<<num_indices<<" indices..."<<endl;
  for(int i = 0; i < num_indices; i++)
  {
    cout<<"\t\t"<<i<<endl;
  }
}

void StaticMesh::init()
{
  //create openGL buffer objects
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(StaticMeshVertex) * num_vertices, vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, indices, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void StaticMesh::render()
{
  //TODO: use DrawCall objects
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(StaticMeshVertex), (void *)0);
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(StaticMeshVertex), (void *)(sizeof(float) * 3));
  //glEnableClientState(GL_COLOR_ARRAY);
  //glColorPointer(3, GL_FLOAT, sizeof(StaticMeshVertex), (void *)(sizeof(float) * 6));
  glClientActiveTexture(GL_TEXTURE0);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(StaticMeshVertex), (void *)(sizeof(float) * 6));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glColor3f(1.0f, 1.0f, 1.0f);
  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void *)(sizeof(unsigned int) * 0));

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}
