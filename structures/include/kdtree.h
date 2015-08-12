#ifndef __KDTREE_2D_H__
#define __KDTREE_2D_H__

#include <vector>

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
using namespace std;

namespace Structures
{
  //class KDTree3D<T>;

  template <class T>
  class KDData3D
  {
  public:
    Float3 p;
    T d;
  };

  template <class T>
  class KDNode3D
  {
    //friend class typename KDTree3D<T>;
  public:
    KDNode3D() : data(T()) { left = right = NULL; }
    ~KDNode3D() {}
  //private:
    Float3 p;
    KDNode3D<T> *left, *right;

    T data;
  };

  template <class T>
  struct KDCompare
  {
    inline bool operator()(KDData3D<T> &a, KDData3D<T> &b)
    {
      return a.p[axis] < b.p[axis];
    }
    int axis;
  };

  template <class T>
  class KDTree3D
  {
  public:
    KDTree3D() { root = NULL; }
    ~KDTree3D() {}

    void add_element(T &data, Float3 pt)
    {
      KDData3D<T> new_element;
      new_element.p = pt;
      new_element.d = data;
      elements.push_back(new_element);
    }

    void build_tree()
    {
      root = build_tree_helper(0, elements.begin(), elements.end());
    }

private:
    KDNode3D<T> *build_tree_helper(const int axis,
                                   typename vector<KDData3D<T> >::iterator a,
                                   typename vector<KDData3D<T> >::iterator b)
    {
      //sort the elements by the current axis and find the median index
      KDCompare<T> kdc;
      kdc.axis = axis;
      std::sort(a, b, kdc);

      typename vector<KDData3D<T> >::iterator median = a + (b - a) / 2;

      KDNode3D<T> *new_node = new KDNode3D<T>;

      int new_axis = (axis + 1) % 3;
      if(a != median)
      {
        new_node->left = build_tree_helper(new_axis, a, median);
      }
      if(median != b)
      {
        new_node->right = build_tree_helper(new_axis, median, b);
      }

      return new_node;
    }

    std::vector<KDData3D<T> > elements;
    KDNode3D<T> *root;
  };
}

#endif //__KDTREE_2D_H__
