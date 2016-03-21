#include <mxml.h>
#include <assert.h>

#include "bakery.h"

using namespace Tool;
using namespace std;
using namespace Math;

void Bakery::init()
{
  smb.init();
  bpb.init();
}

void Bakery::bake(std::string fname, std::string out_fname)
{
  //extract the file extension
  //march backwards from the end of the string
  int dot_idx = fname.size() - 1;
  for(; fname[dot_idx] != '.'; dot_idx--) {}
  std::string fname_ext = fname.substr(dot_idx + 1, fname.size() - dot_idx);

  if(fname_ext == std::string("bphys"))
  {
    cout<<"Baking Blender physics file to texture..."<<endl;
    FILE *fp = fopen(fname.c_str(), "rb");
    if(fp)
    {
      bpb.bake(fp);
    }
    else
    {
      cerr<<"Bakery::bake() - Could not open file! "<<endl<<"\t"<<fname.c_str()<<endl;
    }
  }
  else
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
}
