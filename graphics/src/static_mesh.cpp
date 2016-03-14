#include <iostream>

#include <assert.h>
#include <string.h>

//#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


#include <libxml/parser.h>
#include <libxml/tree.h>

#include "static_mesh.h"

#define RENDER_METHOD_VBO   true

using namespace std;

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

bool StaticMesh::read_from_file(FILE *f)
{
  int version;
  fread(&version, sizeof(int), 1, f);

  int num_verts;
  fread(&num_verts, sizeof(int), 1, f);
  fread(vertices)

  int num_indices;
}

bool StaticMesh::make_render_data()
{
    //1. we need to make an array for each material of all the faces
    //   that use that material
    cout<<"materials.size(): "<<materials.size()<<endl;

    int mat_counter = 0;
    std::vector<Material *>::iterator mi;
    for(mi = materials.begin(); mi != materials.end(); mi++)
    {
      DrawCall dc;
      dc.m = *mi;
      dc.m->init();

      std::vector<MeshFace> mat_face_list;

      std::vector<MeshFace>::iterator fi;
      for(fi = faces.begin(); fi != faces.end(); fi++)
      {
        MeshFace f = *fi;
        if(f.mat_id == mat_counter)
        {
          mat_face_list.push_back(f);
        }
      }

      dc.num_indices = 3 * mat_face_list.size();
      GLushort *gl_indices = new GLushort[dc.num_indices * 3];
      assert(gl_indices);

      int index_data_size = 3 * mat_face_list.size() * sizeof(GLushort);

      //std::vector<MeshFace>::iterator fi;
      int buffer_idx = 0;
      cout<<"num_faces: "<<faces.size()<<endl;
      for(fi = mat_face_list.begin(); fi != mat_face_list.end(); fi++)
      {
        MeshFace f = *(fi);
        assert(f.vertex_indices.size() == 3);

        gl_indices[buffer_idx++] = f.vertex_indices[0];
        gl_indices[buffer_idx++] = f.vertex_indices[1];
        gl_indices[buffer_idx++] = f.vertex_indices[2];
      }
      cout<<"buffer_idx: "<<buffer_idx<<endl;

      //int index_data_size = sizeof(GLushort) * 3 * faces.size();

      glGenBuffersARB(1, &dc.ibo);
      assert(dc.ibo != 0);
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, dc.ibo);
      glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, index_data_size, gl_indices, GL_STATIC_DRAW_ARB);

      delete gl_indices;

      dc.m = materials[mat_counter++];
      draw_calls.push_back(dc);
    }

    //vertex buffer
    GLfloat *gl_verts = new GLfloat[3 * vertices.size()];
    int vertex_data_size = sizeof(GLfloat) * 3 * vertices.size();

    //normal buffer
    GLfloat *gl_normals = new GLfloat[3 * vertices.size()];
    int normal_data_size = sizeof(GLfloat) * 3 * vertices.size();

    int buffer_idx = 0;
    std::vector<MeshVertex>::iterator vi;
    for(vi = vertices.begin(); vi != vertices.end(); vi++)
    {
      MeshVertex v = *(vi);

      gl_normals[buffer_idx] = v.normal[0];
      gl_verts[buffer_idx++] = v.pos[0];

      gl_normals[buffer_idx] = v.normal[1];
      gl_verts[buffer_idx++] = v.pos[1];

      gl_normals[buffer_idx] = v.normal[1];
      gl_verts[buffer_idx++] = v.pos[2];
    }

    cout<<"num_vertices: "<<vertices.size()<<endl;
    cout<<"buffer_idx: "<<buffer_idx<<endl;

    glGenBuffersARB(1, &vbo);
    assert(vbo != 0);
    //cout<<"vbo: "<<(int)vbo<<endl;
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertex_data_size, gl_verts, GL_STATIC_DRAW_ARB);

    glGenBuffersARB(1, &nbo);
    assert(nbo != 0);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, nbo);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, normal_data_size, gl_normals, GL_STATIC_DRAW_ARB);

    delete [] gl_verts;
    delete [] gl_normals;

    return true;
}

bool StaticMesh::init()
{
  if(RENDER_METHOD_VBO)
  {
    make_render_data();
  }
  else
  {
    //build display list
    display_list = glGenLists(1);
    glNewList(display_list, GL_COMPILE);
    {
      std::vector<MeshFace>::iterator fi;
      for(fi = faces.begin(); fi != faces.end(); fi++)
      {
        MeshFace f = *(fi);
        glBegin(GL_TRIANGLES);

        std::vector<int>::iterator vi;
        for(vi = f.vertex_indices.begin(); vi != f.vertex_indices.end(); vi++)
        {
          int v_idx = *(vi);
          MeshVertex vert = vertices[v_idx];
          glNormal3f(f.normal[0], f.normal[1], f.normal[2]);
          glVertex3f(vert.pos[0], vert.pos[1], vert.pos[2]);
        }
        glEnd();
      }
    }
    glEndList();
  }

  return true;
}

void StaticMesh::render_gl()
{
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
            DrawCall dc = *di;
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
}
