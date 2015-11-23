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

bool StaticMesh::read_from_file(const char *filename)
{
  //return true;
  int file_version = -1;

  xmlDocPtr doc;
  doc = xmlReadFile(filename, NULL, 0);
  if(doc == NULL)
  {
    return false;
  }

  //start parsing the xml file
  xmlNode *xml_node = xmlDocGetRootElement(doc);
  if(xml_node->type == XML_ELEMENT_NODE)
  {
    //cout<<xml_node->name<<endl;
    if(!xmlStrcmp(xml_node->name, (const xmlChar *)"static_mesh"))
    {
            //read the properties
      xmlAttr *xml_attribute = NULL;
      for(xml_attribute = xml_node->properties; xml_attribute; xml_attribute = xml_attribute->next)
      {
        if(!xmlStrcmp(xml_attribute->name, (const xmlChar *)"version"))
        {
          xmlChar* value = xmlNodeListGetString(xml_node->doc, xml_attribute->children, 1);
          //do something with value
          file_version = atoi((char *)value);
          xmlFree(value);
        }
        xmlNode *node = NULL;
        for(node = xml_node->children; node; node = node->next)
        {
          assert(node);

          //material
          if(!xmlStrcmp(node->name, (const xmlChar *)"material"))
          {
            xmlNode *mat_node = NULL;
            Material *mat = new Material;
            mat->enable_lighting(true);
            mat->enable_blending(false);
            for(mat_node = node->children; mat_node; mat_node = mat_node->next)
            {
              if(!xmlStrcmp(mat_node->name, (const xmlChar *)"diffuse_color"))
              {
                Float3 diffuse_color;
                char *val = strtok((char *)mat_node->children->content, " ");
                diffuse_color[0] = atof(val);
                val = strtok(NULL, " ");
                diffuse_color[1] = atof(val);
                val = strtok(NULL, " ");
                diffuse_color[2] = atof(val);

                mat->set_diff_rgb(diffuse_color);
              }
            }
            materials.push_back(mat);
          }
          //vertex
          else if(!xmlStrcmp(node->name, (const xmlChar *)"vertex"))
          {
            xmlNode *vert_node = NULL;
            MeshVertex v;

            for(vert_node = node->children; vert_node; vert_node = vert_node->next)
            {
              if(!xmlStrcmp(vert_node->name, (const xmlChar *)"vert_pos"))
              {
                char *val = strtok((char *)vert_node->children->content, " ");
                v.pos[0] = atof(val);
                val = strtok(NULL, " ");
                v.pos[1] = atof(val);
                val = strtok(NULL, " ");
                v.pos[2] = atof(val);
              }
              else if(!xmlStrcmp(vert_node->name, (const xmlChar *)"normal"))
              {
                char *val = strtok((char *)vert_node->children->content, " ");
                v.normal[0] = atof(val);
                val = strtok(NULL, " ");
                v.normal[1] = atof(val);
                val = strtok(NULL, " ");
                v.normal[2] = atof(val);
              }
            }

            vertices.push_back(v);
          }

          //face
          if(!xmlStrcmp(node->name, (const xmlChar *)"face"))
          {
            MeshFace f;
            f.normal[0] = f.normal[1] = f.normal[2];
            xmlNode *face_node = NULL;
            for(face_node = node->children; face_node; face_node = face_node->next)
            {
              if(!xmlStrcmp(face_node->name, (const xmlChar *)"v_idx"))
              {
                int v_idx = atoi((char *)face_node->children->content);
                f.vertex_indices.push_back(v_idx);
              }
              if(!xmlStrcmp(face_node->name, (const xmlChar *)"mat_idx"))
              {
                f.mat_id = atoi((char *)face_node->children->content);
              }
              else if(!xmlStrcmp(face_node->name, (const xmlChar *)"normal"))
              {
                char *val = strtok((char *)face_node->children->content, " ");
                                    f.normal[0] = atof(val);
                                    val = strtok(NULL, " ");
                                    f.normal[1] = atof(val);
                                    val = strtok(NULL, " ");
                                    f.normal[2] = atof(val);
              }
            }
            faces.push_back(f);
          }
        }
      }
    }
  }
  xmlFreeDoc(doc);

  //make_render_data();

  return true;
}

bool StaticMesh::read_from_file_data(StaticMeshFileData *smfd)
{
    //Material *materials = new Material[num_materials];
    unsigned char *data_ptr = (unsigned char *)smfd->materials;
    for(int i = 0; i < smfd->num_materials; i++)
    {
      Material *mat = new Material;
      mat->enable_blending(false);
      mat->enable_lighting(true);

      MaterialFileData *mfd = (MaterialFileData *)data_ptr;
      mat->set_diff_rgb(mfd->diffuse_rgb);

      //cout<<mfd->diffuse_rgb[0]<<", "<<mfd->diffuse_rgb[1]<<", "<<mfd->diffuse_rgb[2]<<endl;

      //cout<<"num_textures: "<<mfd->num_textures<<endl;

      //NEVER CHANGE THIS
      int *tex_idx = (int *)(&mfd->tex_idx);

      for(int j = 0; j < mfd->num_textures; j++)
      {
        assert(mat);
        mat->add_tex_idx(tex_idx[j]);
      }

      //cout<<"got here!"<<endl;

      materials.push_back(mat);

      data_ptr += sizeof(Float3); //diffuse_rgb
      data_ptr += sizeof(int);    //num_textures
      data_ptr += sizeof(int) * mfd->num_textures;
    }

    for(int i = 0; i < smfd->num_vertices; i++)
    {
      MeshVertex mv;
      mv.pos = smfd->vertices[i].position;
      mv.normal = smfd->vertices[i].normal;

      vertices.push_back(mv);
    }

    for(int i = 0; i < smfd->num_faces; i++)
    {
      MeshFace mf;
      mf.mat_id = smfd->faces[i].mat_idx;
      mf.vertex_indices.push_back(smfd->faces[i].vert_idx[0]);
      mf.vertex_indices.push_back(smfd->faces[i].vert_idx[1]);
      mf.vertex_indices.push_back(smfd->faces[i].vert_idx[2]);
      mf.normal = smfd->faces[i].face_normal;

      faces.push_back(mf);
    }

    make_render_data();

    return true;
}

bool StaticMesh::fixup_materials(World *world)
{
  std::vector<Material *>::iterator mi;
  for(mi = materials.begin(); mi != materials.end(); mi++)
  {
    Material *m = *mi;

    std::vector<int>::iterator ti;
    /*for(ti = texture_ids.begin(); ti != texture_ids.end(); ti++)
    {
        int tex_id = *ti;

        assert(world);
        world->texture_list;
    }*/
  }
  return true;
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
