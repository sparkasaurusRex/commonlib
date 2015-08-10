#ifndef __KDTREE_2D_H__
#define __KDTREE_2D_H__

#include "aabb.h"

//
// consider using an optimized point cloud library such as this:
// http://pointclouds.org/
// https://en.wikipedia.org/wiki/Point_Cloud_Library
//
//
// reference for KD-tree implementation:
// http://rosettacode.org/wiki/K-d_tree
//

using namespace Math;

namespace Structures
{
  class KDTree2D<T>;

  class KDNode2D
  {
    friend class KDTree2D;
    KDNode2D() { data = left = right = NULL; }
    ~KDNode2D() {}
  private:
    Float2 p;
    KDNode2D *left, *right;

    T data;
  };

  class KDTree2D
  {
  public:
    KDTree2D();
    ~KDTree2D();

    KDNode2D *build_tree();

  protected:
    int num_nodes;
    KDNode2D *nodes_array;

    KDNode2D *root;
  };
}

#endif //__KDTREE_2D_H__
