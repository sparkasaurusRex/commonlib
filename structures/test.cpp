#include <iostream>
#include <string>
#include <vector>

#include "kdtree.h"

//using namespace std;
using namespace Structures;

int main(int argc, char **argv)
{
  std::vector<int> bar;
  std::string foo("meow meow meow");
  std::string foo2("sdfsdf");
  int x = 8;

  KDTree3D<std::string> kd;
  kd.add_element(foo, Float3(1.0f, 0.5f, 0.2f));
  kd.add_element(foo2, Float3(0.0f, 1.0f, -0.5f));
  kd.add_element(foo, Float3(0.5f, -1.0f, 3.0f));

  kd.build_tree();

  return 0;
}
