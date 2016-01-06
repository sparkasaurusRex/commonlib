#include <assert.h>
#include "gpu_voronoi.h"

using namespace Math;

GPUVoronoiSphere::GPUVoronoiSphere(const GLuint num_seg)
{
  num_cone_segments = num_seg;
  cone_vbo = 0;
  cone_ibo = 0;

  fbo_res[0] = 2048;
  fbo_res[1] = 1024;

  voronoi_diagram_fbo = 0;
  voronoi_diagram_tex = 0;

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
}

GPUVoronoiSphere::~GPUVoronoiSphere() {}

void GPUVoronoiSphere::init()
{
  GLuint internal_format = GL_RGBA_FLOAT32_ATI;

  //allocate the voronoi diagram texture and frame buffer object
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
               GL_RGBA, //GL_RGBA16F_ARB,      //RGB = xyz (pos), A = ?
               GL_FLOAT,//GL_UNSIGNED_BYTE,
               NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  assert(glIsTexture(voronoi_diagram_tex) == GL_TRUE);
}

void GPUVoronoiSphere::deinit()
{

}

void GPUVoronoiSphere::reset()
{
  sites.clear();
}

void GPUVoronoiSphere::add_point(Float3 pt)
{
  sites.push_back(pt);
}
