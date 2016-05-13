#ifndef __KDTREE_H__
#define __KDTREE_H__

#include <vector>
#include "math_utility.h"


/*
 * KDTree for 3-dimensional points. By Ellis Hoag.
 * TODO: Currently you can insert and remove nodes
 * but the tree will not necessarily remain balanced.
 * After several insert/remove calls, this will become
 * slower and slower.
 * Ideally we could implement a balance function for each
 * insert/remove. Or we could periodically remake the tree
 * when the balance becomes too bad.
 */

namespace Structures    {
  template <class T>
  struct KDNode
  {
    KDNode(Math::Float3 v = Math::Float3(0.f, 0.f, 0.f), T t = T()) : data(t), value(v), left(NULL), right(NULL) {}
    ~KDNode() { delete left; delete right; }

    T data;
    Math::Float3 value;
    KDNode<T> *left;
    KDNode<T> *right;
  };

  template <class T>
  class KDTree3D
  {
  public:
    KDTree3D() { root = NULL; }
    ~KDTree3D() { delete root; }

    void insert_element(Math::Float3 element, T data) { insertElementHelper(element, data, root, 0); }
    void remove_element(Math::Float3 element) { removeElementHelper(element, root, 0); }
    void reset() { delete root; root = NULL; }

    KDNode<T> *find_nearest_neighbor(Math::Float3 query)
    {
      KDNode<T> *closest = NULL;
      float best_d2 = findNearestNeighborHelper(&closest, &query, root, 0);
      return closest;
    }

  private:
    KDNode<T> *root;
    /*
     * epsilon is used to determine if two Float3's are equal.
     * infinity is used in the findMin function
     */
    float epsilon = 0.0000000001f;
    float infinity = (float)99999999999999;

    int DIM = 3;

    Math::Float3 findMin(KDNode<T> * cRoot, int searchDimension, int currentDimension)
    {
      if(cRoot == NULL)
      {
        return Math::Float3(infinity, infinity, infinity);
      }
      if(cRoot->left == NULL && cRoot->right == NULL)
      {
        //Base case
        return cRoot->value;
      }

      int nextDimension = (currentDimension + 1) % DIM;

      if(currentDimension == searchDimension)
      {
        //We only visit the left subtree
        if (cRoot->left == NULL)
        {
          return cRoot->value;
        }
        else
        {
          return findMin(cRoot->left, searchDimension, nextDimension);
        }
      }
      else
      {
        Math::Float3 left = findMin(cRoot->left, searchDimension, nextDimension);
        Math::Float3 right = findMin(cRoot->right, searchDimension, nextDimension);

        return (left[searchDimension] < right[searchDimension]) ? left : right;
      }
    }

    void removeElementHelper(Math::Float3 element, KDNode<T> * & cRoot, int currentDimension)
    {
      if(cRoot == NULL)
      {
        //Our point is not found. Do nothing.
        return;
      }

      int nextDimension = (currentDimension + 1) % DIM;

      if(dist_squared(cRoot->value, element) < epsilon)
      {
        //This is the node we want to delete
        if(cRoot->right != NULL)
        {
          cRoot->value = findMin(cRoot->right, currentDimension, nextDimension);
          removeElementHelper(cRoot->value, cRoot->right, nextDimension);
        }
        else if(cRoot->left != NULL)
        {
          cRoot->value = findMin(cRoot->left, currentDimension, nextDimension);
          removeElementHelper(cRoot->value, cRoot->left, nextDimension);
        }
        else
        {
          delete cRoot;
          cRoot = NULL;
        }
      }   //else we need to search for our element
      else if(cRoot->value[currentDimension] > element[currentDimension])
      {
        removeElementHelper(element, cRoot->left, nextDimension);
      }
      else
      {
        removeElementHelper(element, cRoot->right, nextDimension);
      }
    }

    void insertElementHelper(Math::Float3 value, T data, KDNode<T> * & cRoot, int currentDimension)
    {

      if(cRoot == NULL)
      {
        cRoot = new KDNode<T>(value, data);
        return;
      }

      int nextDimension = (currentDimension + 1) % DIM;

      if(value[currentDimension] < cRoot->value[currentDimension])
      {
        //Insert left
        insertElementHelper(value, data, cRoot->left, nextDimension);
      }
      else
      {
        //Insert right
        insertElementHelper(value, data, cRoot->right, nextDimension);
      }
    }

    float findNearestNeighborHelper(KDNode<T> **closest, Math::Float3 *query, KDNode<T> *cRoot, int currentDimension)
    {
      if(cRoot == NULL)
      {
        *closest = NULL;
        return -1.0f;
      }
      if(cRoot->left == NULL && cRoot->right == NULL)
      {
        *closest = cRoot;
        return dist_squared(*query, cRoot->value);
      }

      KDNode<T> *currentBestNode;
      int nextDimension = (currentDimension + 1) % DIM;

      bool checkedLeftHyperrectangle;

      float d2_query_cbn = -1.0f;
      if(query->_val[currentDimension] < cRoot->value[currentDimension])
      {
        //Search left
        d2_query_cbn = findNearestNeighborHelper(&currentBestNode, query, cRoot->left, nextDimension);
        checkedLeftHyperrectangle = true;
      }
      else
      {
        //Search right
        d2_query_cbn = findNearestNeighborHelper(&currentBestNode, query, cRoot->right, nextDimension);
        checkedLeftHyperrectangle = false;
      }

      float best_d2 = d2_query_cbn;

      float x, y, z;
      if(currentBestNode != NULL)
      {
        x = query->_val[0] - currentBestNode->value._val[0];
        y = query->_val[1] - currentBestNode->value._val[1];
        z = query->_val[2] - currentBestNode->value._val[2];
        d2_query_cbn = x * x + y * y + z * z;
      }// ? dist_squared(query, currentBestNode->value) : -1.0f;

      //float d2_query_croot = dist_squared(query, cRoot->value);
      x = query->_val[0] - cRoot->value._val[0];
      y = query->_val[1] - cRoot->value._val[1];
      z = query->_val[2] - cRoot->value._val[2];
      float d2_query_croot = x * x + y * y + z * z;

      if(currentBestNode == NULL || d2_query_croot < d2_query_cbn)
      {
        currentBestNode = cRoot;
        d2_query_cbn = d2_query_croot;
      }

      float distSqrdToSplittingPlane = query->_val[currentDimension] - cRoot->value._val[currentDimension];
      distSqrdToSplittingPlane *= distSqrdToSplittingPlane;

      if(distSqrdToSplittingPlane < d2_query_cbn)
      {
        //We need to check the other hyperrectangle
        KDNode<T> *possibleBest;
        float d2_query_pb = -1.0f;
        if(checkedLeftHyperrectangle)
        {
          d2_query_pb = findNearestNeighborHelper(&possibleBest, query, cRoot->right, nextDimension);
        }
        else
        {
          d2_query_pb = findNearestNeighborHelper(&possibleBest, query, cRoot->left, nextDimension);
        }

        if(possibleBest != NULL && (d2_query_pb < d2_query_cbn))
        //if(possibleBest != NULL && dist_squared(*query, possibleBest->value) < d2_query_cbn)
        {
          currentBestNode = possibleBest;
          best_d2 = d2_query_pb;
        }
      }
      best_d2 = d2_query_cbn;
      *closest = currentBestNode;
      return best_d2;
    }
  };
}

#endif //__KDTREE_H__
