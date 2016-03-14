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
    vbo = nbo = 0;
}

StaticMesh::~StaticMesh()
{
  glDeleteBuffersARB(1, &vbo);
  //glDeleteBuffersARB(1, &ibo);
  glDeleteBuffersARB(1, &nbo);
}

void StaticMesh::read_from_file(FILE *f)
{
  int version;
  fread(&version, sizeof(int), 1, f);

  int num_verts;
  fread(&num_verts, sizeof(int), 1, f);
  vertices = new StaticMeshVertex[num_verts];
  fread(vertices, sizeof(StaticMeshVertex), num_verts, f);

  cout<<"StaticMesh::read_from_file(): "<<endl;
  cout<<"\t"<<num_verts<<" vertices read"<<endl;

  int num_indices;
}

bool StaticMesh::init()
{
  vbo = 0;
  nbo = 0;
}

void StaticMesh::render_gl()
{
  /*
  if(!RENDER_METHOD_VBO)
  {
    glCallList(display_list);
  }
  else
  {
    //method 2: VBOs
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);

    std::vector<DrawCall>::iterator di;
    for(di = draw_calls.begin(); di != draw_calls.end(); di++)
    {
      DrawCall<StaticMeshVertex> dc = *di;
      dc.m->render_gl();

      glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, dc.ibo);

      glVertexPointer(3, GL_FLOAT, 0, (char *)NULL);
      glNormalPointer(GL_FLOAT, 0, (char *)NULL);

      glDrawElements(GL_TRIANGLES, dc.num_indices, GL_UNSIGNED_SHORT, 0);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glPopAttrib();
  }
  */
}
