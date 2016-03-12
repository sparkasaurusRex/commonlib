#include <iostream>

#include "bake_static_mesh.h"

using namespace Tool;
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
}
