#include <iostream>
#include <assert.h>

#include "tool.h"
#include "static_mesh_baker.h"

using namespace Tool;
using namespace Graphics;
using namespace std;

StaticMeshBaker::StaticMeshBaker()
{

}

void StaticMeshBaker::bake(mxml_node_t *tree)
{
  cout<<"\tParsing static mesh xml..."<<endl;

  //read all the materials
  mxml_node_t *material_node = NULL;
  mxml_node_t *start_node = tree;
  do
  {
    material_node = mxmlFindElement(start_node, tree, "material", NULL, NULL, MXML_DESCEND);

    if(material_node)
    {
      mxml_node_t *diff_node = mxmlFindElement(material_node, material_node, "diffuse_color", NULL, NULL, MXML_DESCEND);
      assert(diff_node);
      Float3 diff_color = mxml_read_float3(diff_node->child);
      cout<<"\tdiffuse: "<<diff_color<<endl;
    }

    //read all the textures used by this material
    mxml_node_t *tex_node = NULL;
    mxml_node_t *t_start_node = material_node;

    do
    {
      tex_node = mxmlFindElement(t_start_node, material_node, "texture", NULL, NULL, MXML_DESCEND);
      if(tex_node)
      {
        //TODO
      }
      t_start_node = tex_node;
    } while(tex_node);

    start_node = material_node;
  } while(material_node);

  //read all the vertices
  std::vector<Float3> vertex_xyz;
  mxml_node_t *vert_node = NULL;
  start_node = tree;
  do
  {
    vert_node = mxmlFindElement(start_node, tree, "vertex", NULL, NULL, MXML_DESCEND);
    if(vert_node)
    {
      mxml_node_t *pos_node = mxmlFindElement(vert_node, vert_node, "vert_pos", NULL, NULL, MXML_DESCEND);
      assert(pos_node);
      Float3 vert_pos = mxml_read_float3(pos_node->child);
      vertex_xyz.push_back(vert_pos);
      cout<<"\tv: "<<vert_pos<<endl;

      mxml_node_t *norm_node = mxmlFindElement(vert_node, vert_node, "normal", NULL, NULL, MXML_DESCEND);
      assert(norm_node);
      Float3 vert_normal = mxml_read_float3(norm_node->child);
      cout<<"\tn: "<<vert_normal<<endl;




      start_node = vert_node;
    }
  } while(vert_node);



  //read all the faces
  std::vector<MeshFace> mesh_faces;
  mxml_node_t *face_node = NULL;
  start_node = tree;
  do
  {
    face_node = mxmlFindElement(start_node, tree, "face", NULL, NULL, MXML_DESCEND);
    if(face_node)
    {
      MeshFace mf;

      mxml_node_t *face_idx_node = mxmlFindElement(face_node, face_node, "face_idx", NULL, NULL, MXML_DESCEND);
      assert(face_idx_node);

      int face_idx = atoi(face_idx_node->child->value.text.string);
      cout<<"\tface id: "<<face_idx<<endl;

      mxml_node_t *mat_idx_node = mxmlFindElement(face_node, face_node, "mat_idx", NULL, NULL, MXML_DESCEND);
      int mat_idx = atoi(mat_idx_node->child->value.text.string);
      cout<<"\t\tmat_idx: "<<mat_idx<<endl;

      int v_idx[3] = { -1, -1, -1 };
      Float3 col[3];
      Float2 uvs[3];

      mxml_node_t *vidx_node = face_node;
      mxml_node_t *uv_node = face_node;
      mxml_node_t *col_node = face_node;
      for(int i = 0; i < 3; i++)
      {
        //uvs
        uv_node = mxmlFindElement(uv_node, face_node, "uv", NULL, NULL, MXML_DESCEND);
        assert(uv_node);

        uvs[i] = mxml_read_float2(uv_node->child);
        cout<<"\t\tuv: "<<uvs[i]<<endl;

        //vertex color
        col_node = mxmlFindElement(col_node, face_node, "col", NULL, NULL, MXML_DESCEND);
        assert(col_node);
        col[i] = mxml_read_float3(col_node->child);
        cout<<"\t\trgb: "<<col[i]<<endl;

        //vertex index
        vidx_node = mxmlFindElement(vidx_node, face_node, "v_idx", NULL, NULL, MXML_DESCEND);
        assert(vidx_node);
        v_idx[i] = atoi(vidx_node->child->value.text.string);
        cout<<"\t\tvidx: "<<v_idx[i]<<endl;
      }

      //face normal
      mxml_node_t *norm_node = mxmlFindElement(face_node, face_node, "normal", NULL, NULL, MXML_DESCEND);
      assert(norm_node);
      Float3 face_normal = mxml_read_float3(norm_node->child);
      cout<<"\tfn: "<<face_normal<<endl;

      mesh_faces.push_back(mf);

      start_node = face_node;
    }
  } while(face_node);

  //build render data
  unsigned int *indices = new unsigned int[mesh_faces.size()];

  delete indices;
}
