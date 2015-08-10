#include "kdtree.h"

using namespace Structures;

KDTree2D::KDTree2D() {}

KDTree2D::~KDTree2D() {}

KDNode2D *KDTree2D::build_tree(KDNode2D *t, int len, int i)
{
  return NULL;
  /*KDNode2D *n;
  if(!len) return NULL;
  if((n == find_median(t, t + len, i)))
  {
    i = (i + 1) % 2;
    n->left = make_tree(t, n - t, i);
    n->right = make_tree(n + 1, t + len - (n + 1), i);
  }
  return n
  */
}
