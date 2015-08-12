#ifndef __KDTREE_2D_H__
#define __KDTREE_2D_H__

#include <vector>
#include <iostream>
#include <assert.h>

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
    KDData3D() : d(T()) {}
    Float3 p;
    T d;
  };

  template <class T>
  class KDNode3D
  {
    //friend class typename KDTree3D<T>;
  public:
    KDNode3D() : data() { left = right = NULL; }
    ~KDNode3D() {}
  //private:
    //Float3 p;
    KDNode3D<T> *left, *right;

    //T data;
    KDData3D<T> data;
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
      assert(tree_size() == elements.size());
    }

    KDData3D<T> find_nearest_neighbor(const Float3 p, float &best_d2)
    {
      best_d2 = dist_squared(p, root->data.p);
      KDNode3D<T> *nearest = find_nearest_neighbor_helper(0, root, p, best_d2);
      return nearest->data;
    }

    int tree_size(KDNode3D<T> *r = NULL) const
    {
      if(r)
      {
        int s = 1;
        if(r->left) { s += tree_size(r->left); }
        if(r->right) { s += tree_size(r->right); }
        return s;
      }
      else
      {
        return tree_size(root);
      }
    }
private:
    KDNode3D<T> *build_tree_helper(const int axis,
                                   typename vector<KDData3D<T> >::iterator a,
                                   typename vector<KDData3D<T> >::iterator b)
    {
      if(a == b) { return NULL; }
      //sort the elements by the current axis and find the median index
      KDCompare<T> kdc;
      kdc.axis = axis;
      std::sort(a, b, kdc);
      typename vector<KDData3D<T> >::iterator median = a + (b - a) / 2;

      /*
        cout<<"build_tree_helper()"<<endl;
        cout<<"a: "<<a - elements.begin()<<endl;
        cout<<"b: "<<b - elements.begin()<<endl;
        //cout<<"b - a: "<<(b-a)<<endl;
        cout<<"axis: "<<axis<<endl;
      */

      KDNode3D<T> *new_node = new KDNode3D<T>;
      new_node->data = *median;

      /*
        cout<<"new node: "<<endl;
        cout<<"\t"<<new_node->data.d<<endl;
        cout<<"\t"<<new_node->data.p<<endl;
      */

      if(b - a <= 1) { return new_node; } //leaf

      int new_axis = (axis + 1) % 3;
      new_node->left = build_tree_helper(new_axis, a, median);
      new_node->right = build_tree_helper(new_axis, median + 1, b);

      return new_node;
    }

    KDNode3D<T> *find_nearest_neighbor_helper(int axis, KDNode3D<T> *r, const Float3 p, float &best_d2)
    {
      if(!r) { return NULL; }
      int new_axis = (axis + 1) % 3;

      if(p[axis] < r->data.p[axis] && r->left)
      {
        float l_d2 = dist_squared(p, r->left->data.p);
        if(l_d2 < best_d2)
        {
          best_d2 = l_d2;
          return find_nearest_neighbor_helper(new_axis, r->left, p, best_d2);
        }
      }
      else if(p[axis] > r->data.p[axis] && r->right)
      {
        float r_d2 = dist_squared(p, r->right->data.p);
        if(r_d2 < best_d2)
        {
          best_d2 = r_d2;
          return find_nearest_neighbor_helper(new_axis, r->right, p, best_d2);
        }
      }

      return r;

      //go left
      /*if(p[axis] < r->data.p[axis])
      {
        if(r->left)
        {
          return find_nearest_neighbor_helper(new_axis, r->left, p);
        }
        else
        {
          return r;
        }
      }
      //go right
      else if(p[axis] > r->data.p[axis])
      {
        if(r->right)
        {
          return find_nearest_neighbor_helper(new_axis, r->right, p);
        }
        else
        {
          return r;
        }
      }

      //must be equal?
      return r;*/
    }
    std::vector<KDData3D<T> > elements;
    KDNode3D<T> *root;
  };
}

#endif //__KDTREE_2D_H__
