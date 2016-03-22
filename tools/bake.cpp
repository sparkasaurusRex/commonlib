#include <iostream>

#include "bakery.h"

using namespace std;
using namespace Tool;

int main(int argc, char **argv)
{
  Bakery bakery;
  bakery.init();

  string in_fname, out_fname;

  if(argc > 1)
  {
    for(int i = 1; i < argc; i++)
    {
      in_fname = argv[1];
      out_fname = make_nice_filename(std::string(""), std::string(""), i);
      cout<<"Baking "<<in_fname.c_str()<<"..."<<endl;
      bakery.bake(in_fname, out_fname);
    }
  }

  return 0;
}
