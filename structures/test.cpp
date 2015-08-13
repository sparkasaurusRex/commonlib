#include <iostream>
#include <string>
#include <vector>

#include "kdtree.h"
#include "math_utility.h"

//using namespace std;
using namespace Structures;
using namespace Math;

int main(int argc, char **argv)
{
  KDTree3D<int> kd;

  Float3 q(0.7f, -0.3f, 1.24f);

  int nearest = -1;
  float min_d = -1.0f;
  for(int i = 0; i < 20; i++)
  {
    Float3 p(random(-1.0f, 1.0f), random(-1.0f, 1.0f), random(-1.0f, 1.0f));
    cout<<p<<endl;
    float d = distance(p, q);
    cout<<"\tdist: "<<d<<endl;
    if(nearest < 0 || d < min_d)
    {
      min_d = d;
      nearest = i;
    }
    kd.add_element(i, p);
  }

  cout<<"nearest brute force: "<<endl;
  cout<<"\t"<<nearest<<endl;
  cout<<"\t"<<min_d<<endl;

  kd.build_tree();
  cout<<"tree size: "<<kd.tree_size()<<endl;

  float best_d2 = 0.0f;
  KDData3D<int> nearest_node = kd.find_nearest_neighbor(q, best_d2);
  cout<<endl<<endl;
  cout<<"nearest: "<<endl;
  cout<<"\t"<<nearest_node.d<<endl;
  cout<<"\t"<<nearest_node.p<<endl;
  cout<<"\t"<<sqrt(best_d2)<<endl;

  return 0;
}
