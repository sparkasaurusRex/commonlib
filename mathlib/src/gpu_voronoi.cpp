#include <assert.h>
#include "gpu_voronoi.h"

using namespace Math;

GPUVoronoi2D::GPUVoronoi2D(const GLuint num_seg, const GLuint flags)
{
  behavior_flags = flags;
  num_cone_segments = num_seg;

  cone_vbo = 0;
  cone_ibo = 0;

  fbo_res[0] = 2048;
  fbo_res[1] = 1024;

  depth_fbo = 0;
  voronoi_diagram_fbo = 0;
  voronoi_diagram_tex = 0;

  cone_vertex_data = NULL;
}

GPUVoronoi2D::~GPUVoronoi2D() {}

void GPUVoronoi2D::init()
{
  //allocate vertex data for the cones (GL_TRIANGLE_FAN)
  int num_cone_verts = 1 + num_cone_segments;
  cone_vertex_data = new ConeVert[num_cone_verts];
  int vidx = 0;
  for(int i = 0; i < num_cone_segments; i++)
  {
    float theta = M_PI * (float)i / (float)num_cone_segments;
    cone_vertex_data[vidx].x = cos(theta);
    cone_vertex_data[vidx].y = sin(theta);
    vidx++;
  }

  //allocate the voronoi diagram texture and frame buffer object
  GLuint internal_format = GL_RGBA_FLOAT32_ATI;
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &voronoi_diagram_tex);
  glBindTexture(GL_TEXTURE_2D, voronoi_diagram_tex);
  assert(glIsTexture(voronoi_diagram_tex) == GL_TRUE);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               internal_format,
               fbo_res[0],
               fbo_res[1],
               0,
               GL_RGBA, //GL_RGBA16F_ARB,
               GL_UNSIGNED_BYTE,
               NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  assert(glIsTexture(voronoi_diagram_tex) == GL_TRUE);
}

void GPUVoronoi2D::deinit()
{
  if(cone_vertex_data) { delete cone_vertex_data; }
}

void GPUVoronoi2D::reset()
{
  sites.clear();
}

void GPUVoronoi2D::add_point(Float2 pt)
{
  sites.push_back(pt);
}

void GPUVoronoi2D::build_voronoi_diagram()
{
  //set the render target to the voronoi diagram texture / fbo

  //glClear();

  //render a cone for each site
  for(int i = 0; i < sites.size(); i++)
  {
    //Float3 cone_pos(sites[i][0], sites[i][1], -1.0f);
    glTranslatef(sites[i][0], sites[i][1], -1.0f);

    //render cone
  }

  // set the FBO back to default
}
