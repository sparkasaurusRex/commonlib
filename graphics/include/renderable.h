#pragma once

#include <iostream>
#include "material.h"

namespace Graphics
{
  template <class VertexType>
  class Renderable
  {
  protected:
    Material *material;

    uint32_t num_vertices;
    VertexType *vertex_data;

    uint32_t num_indices;
    uint32_t *index_data;

    GLuint vbo;
    GLuint ibo;

    void allocate_buffers()
    {
      if (vertex_data) { delete vertex_data; }
      if (index_data) { delete index_data; }
      vertex_data = new VertexType[num_vertices];
      index_data = new unsigned int[num_indices];
    }

    void init_buffers()
    {
      //create vbo / ibo
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(VertexType) * num_vertices, vertex_data, GL_DYNAMIC_DRAW);

      glGenBuffers(1, &ibo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, index_data, GL_DYNAMIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  public:
    Renderable() { material = NULL; vertex_data = NULL; index_data = NULL; }
    ~Renderable() {}
    virtual void init() = 0;
    virtual void render(const double game_time)
    {
      std::cout << "mow" << std::endl;
      glUseProgramObjectARB(0);
      glDisable(GL_CULL_FACE);

      glEnable(GL_DEPTH_TEST);
      glDepthRange(0.0f, 1.0f);
      glDepthMask(GL_FALSE);

      material->render();

      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, sizeof(VertexType), (void *)0);

      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer(GL_FLOAT, sizeof(VertexType), (void *)(sizeof(float) * 3));

      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(3, GL_FLOAT, sizeof(VertexType), (void *)(sizeof(float) * 6));
      glClientActiveTexture(GL_TEXTURE0);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, sizeof(VertexType), (void *)(sizeof(float) * 9));

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glDrawElements(GL_QUAD_STRIP, num_indices, GL_UNSIGNED_INT, (void *)0);

      material->cleanup();
    }
    virtual void simulate(const double game_time, const double frame_time) = 0;

    void set_material(Graphics::Material *m) { material = m; }
  };
};
