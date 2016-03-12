#include <iostream>
#include <assert.h>

#include "bake_static_mesh.h"

using namespace Tool;
using namespace std;

StaticMeshBaker::StaticMeshBaker()
{

}

//TEMP - move to an xml lib
Float2 mxml_read_float2(const mxml_node_t *node)
{
    assert(node);
    assert(node->next);

    Float2 val(atof(node->value.text.string),
               atof(node->next->value.text.string));

    return val;
}

Float3 mxml_read_float3(const mxml_node_t *node)
{
    assert(node);
    assert(node->next);
    assert(node->next->next);

    Float3 val(atof(node->value.text.string),
               atof(node->next->value.text.string),
               atof(node->next->next->value.text.string));

    return val;
}

void StaticMeshBaker::bake(mxml_node_t *tree)
{
  cout<<"\tParsing static mesh xml..."<<endl;

  //read all the materials
  mxml_node_t *material_node = NULL;
  mxml_node_t *start_node = tree;
  do
  {
    material_node = mxmlFindElement(start_node,
                                    tree,
                                    "material",
                                    NULL,
                                    NULL,
                                    MXML_DESCEND);

    if(material_node)
    {

    }
  } while(material_node);

  //read all the vertices
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
      cout<<"\t"<<vert_pos<<endl;

      start_node = vert_node;
    }
  } while(vert_node);

  //read all the faces
  mxml_node_t *face_node = NULL;
  start_node = tree;
  do
  {
    face_node = mxmlFindElement(start_node, tree, "face", NULL, NULL, MXML_DESCEND);
    if(face_node)
    {
      mxml_node_t *face_idx_node = mxmlFindElement(face_node, face_node, "face_idx", NULL, NULL, MXML_DESCEND);
      assert(face_idx_node);

      int face_idx = atoi(face_idx_node->child->value.text.string);
      cout<<"\tface id: "<<face_idx<<endl;

      mxml_node_t *mat_idx_node = mxmlFindElement(face_node, face_node, "mat_idx", NULL, NULL, MXML_DESCEND);
      int mat_idx = atoi(mat_idx_node->child->value.text.string);
      cout<<"\tmat_idx: "<<mat_idx<<endl;

      start_node = face_node;
    }
  } while(face_node);
}
