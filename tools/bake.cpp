#include <iostream>

#include "bakery.h"

using namespace std;
using namespace Tool;

int main(int argc, char **argv)
{
  Bakery bakery;

  string fname;

  if(argc > 1)
  {
    fname = argv[1];
    cout<<"Baking "<<fname.c_str()<<"..."<<endl;
    bakery.bake(fname);
  }

  return 0;
}
