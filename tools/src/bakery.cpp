#include <mxml.h>
#include <assert.h>

#include "bakery.h"

using namespace Tool;
using namespace std;
using namespace Math;

void Bakery::bake(std::string fname)
{
  std::string output_fname = fname + ".bin";
  FILE *fp = fopen(fname.c_str(), "r");
  if(fp)
  {
    //unsigned int hash_id = hash(fname.c_str());
    //cout<<"\thash value:"<<hash_id<<endl;

    mxml_node_t *tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
    assert(tree);

    //don't need the file anymore now that we have the xml tree
    fclose(fp);

    mxml_node_t *node;

    //check if this is a static mesh
    node = mxmlFindElement(tree, tree, "static_mesh", "version", NULL, MXML_DESCEND);
    if(node)
    {
      smb.bake(tree, output_fname);
    }
  }
  else
  {
    cerr<<"Bakery::bake() - Could not open file! "<<endl<<"\t"<<fname.c_str()<<endl;
  }
}
