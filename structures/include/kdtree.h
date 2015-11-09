#ifndef __KDTREE_2D_H__
#define __KDTREE_2D_H__

#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>

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
    KDNode3D<T> *left, *right;
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
    KDTree3D() { root = NULL; elements = NULL; num_elements = 0; }
    ~KDTree3D() {}

    void add_element(T &data, Float3 pt)
    {
      KDData3D<T> new_element;
      new_element.p = pt;
      new_element.d = data;
      elements[num_elements++] = new_element;
    }

    void resize_elements(const int new_size)
    {
      if(elements) { delete elements; }
      elements = new KDData3D<T>[new_size];
      num_elements = 0;
    }

    //tear down the tree and deallocate memory
    void reset()
    {
      if(root)
      {
        reset_helper(root);
      }
      num_elements = 0;
    }

    void build_tree()
    {
      root = build_tree_helper(0, 0, num_elements);
    }

    KDData3D<T> *find_nearest_neighbor(const Float3 p, float &best_d2)
    {
      best_d2 = dist_squared(p, root->data.p);
      KDNode3D<T> *nearest = NULL;
      find_nearest_neighbor_helper(0, root, p, &nearest, best_d2);
      if(nearest)
      {
        return &(nearest->data);
      }
      return NULL;
    }

    /*void find_n_nearest_neighbors(int n, const Float3 p, KDData3D<t> *nearest, float *best_d2)
    {
      int num_found = 0;
      best_d2 = dist_squared(p, root->data.p);
      KDNode3D<T> *nearest = NULL;

      find_nearest_neighbor_helper
    }*/

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

    void print_tree(KDNode3D<T> *r = NULL, std::string prefix = std::string("")) const
    {
      if(r)
      {
        prefix = "..." + prefix;
        cout<<prefix.c_str()<<"pos:  "<<r->data.p<<endl;
        cout<<prefix.c_str()<<"data: "<<r->data.d<<endl;
        if(r->left) { print_tree(r->left, prefix); }
        if(r->right) { print_tree(r->right, prefix); }
      }
      else
      {
        print_tree(root, std::string(">"));
      }
    }

    void test_quicksort()
    {
      quicksort_along_axis(1, 0, num_elements - 1);

      for(int i = 0; i < num_elements; i++)
      {
        cout<<elements[i].p<<endl;
      }
    }
private:
    void swap(int a, int b)
    {
      KDData3D<T> t = elements[a];
      elements[a] = elements[b];
      elements[b] = t;
    }

    int partition(const int axis, int l, int h)
    {
      KDData3D<T> x = elements[h];
      int i = (l - 1);

      for(int j = l; j <= h- 1; j++)
      {
        if(elements[j].p[axis] < x.p[axis])
        {
          i++;
          swap(i, j);
        }
      }
      swap(i + 1, h);
      return (i + 1);
    }

    void quicksort_along_axis(const int axis, int l, int h)
    {
      // Create an auxiliary stack
      int stack[h - l + 1];
      int top = -1;

      // push initial values of l and h to stack
      stack[++top] = l;
      stack[++top] = h;

      // Keep popping from stack while is not empty
      while(top >= 0)
      {
        // Pop h and l
        h = stack[top--];
        l = stack[top--];

        // Set pivot element at its correct position in sorted array
        int p = partition(axis, l, h);

        // If there are elements on left side of pivot, then push left
        // side to stack
        if(p - 1 > l)
        {
          stack[++top] = l;
          stack[++top] = p - 1;
        }

        // If there are elements on right side of pivot, then push right
        // side to stack
        if(p + 1 < h)
        {
          stack[++top] = p + 1;
          stack[++top] = h;
        }
      }
    }

    void reset_helper(KDNode3D<T> *r)
    {
      if(r->left) { reset_helper(r->left); }
      if(r->right) { reset_helper(r->right); }

      delete r;
      return;
    }

    KDNode3D<T> *build_tree_helper(const int axis,
                                   const int a,
                                   const int b)
    {
      if(a == b) { return NULL; }
      //sort the elements by the current axis and find the median index
      quicksort_along_axis(axis, a, b - 1);
      int median = a + (b - a) / 2;

      KDNode3D<T> *new_node = new KDNode3D<T>;
      new_node->data = elements[median];

      if(b - a <= 1) { return new_node; } //leaf

      int new_axis = (axis + 1) % 3;
      new_node->left = build_tree_helper(new_axis, a, median);
      new_node->right = build_tree_helper(new_axis, median + 1, b);

      return new_node;
    }

    void find_nearest_neighbor_helper(int axis, KDNode3D<T> *r, const Float3 p, KDNode3D<T> **best, float &best_d2, int depth = 0)
    {
      if(!r) { return; }
      int new_axis = (axis + 1) % 3;

      float d2 = dist_squared(r->data.p, p);
      float dx = r->data.p[axis] - p[axis];
      float dx2 = dx * dx;

      if(!best || d2 <= best_d2)
      {
        best_d2 = d2;
        *best = r;
      }

      if(best_d2 == 0) { return; }

      find_nearest_neighbor_helper(new_axis, dx > 0 ? r->left : r->right, p, best, best_d2, depth + 1);
      if(dx2 >= best_d2) { return; }
      find_nearest_neighbor_helper(new_axis, dx > 0 ? r->right : r->left, p, best, best_d2, depth + 1);
    }
    KDData3D<T> *elements;
    int num_elements;

    KDNode3D<T> *root;
  };
}

#endif //__KDTREE_2D_H__
