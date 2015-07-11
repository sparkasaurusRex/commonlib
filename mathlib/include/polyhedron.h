#ifndef __POLYHEDRON_H__
#define __POLYHEDRON_H__

class Polyhedron
{
public:
  Polyhedron();
  ~Polyhedron();
protected:
  int num_faces;
  Face3D *faces;
  void **face_data;

  int num_vertices;
  Float3 *vertices;
  void **vertex_data;

  int num_edges;
  Edge3D *edges;
  void **edge_data;
};

#endif //__POLYHEDRON_H__
