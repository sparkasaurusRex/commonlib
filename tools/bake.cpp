#include <iostream>
#include "bakery.h"
#include "platform.h"

using namespace std;
using namespace Tool;

int main(int argc, char **argv)
{
  Bakery bakery;
  bakery.init();

  string in_fname, out_fname;

  char cwd[FILENAME_MAX];
  GETCWD(cwd, sizeof(cwd));
  cout << "working dir: " << cwd << endl;

  if(argc > 1)
  {
    for(int i = 1; i < argc; i++)
    {
      in_fname = argv[i];
      out_fname = make_nice_filename(std::string(""), std::string(""), i);
      cout<<"Baking "<<in_fname.c_str()<<"..."<<endl;
      bakery.bake(in_fname, out_fname);
    }
  }

#if defined (_WIN32)
  Sleep(60 * 1000);
#endif

  return 0;
}
