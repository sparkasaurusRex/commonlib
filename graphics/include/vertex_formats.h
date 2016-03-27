#ifndef __VERTEX_FORMATS_H__
#define __VERTEX_FORMATS_H__

struct VertexP
{
  float x, y, z;
};

struct VertexPN
{
  float x, y, z;
  float nx, ny, nz;
};

struct VertexPNU
{
  float x, y, z;
  float nx, ny, nz;
  float u, v;
};

#endif //__VERTEX_FORMATS_H__
