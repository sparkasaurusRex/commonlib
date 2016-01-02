#ifndef _KDTREE_H_
#define _KDTREE_H_

#include <vector>
#include "math_utility.h"

using namespace Math;
using namespace std;
/*
 * KDTree for 3-dimensional points. By Ellis Hoag.
 * TODO: Currently you can insert and remove nodes
 * but the tree will not necessarily remain balanced.
 * After several insert/remove calls, this will become
 * slower and slower.
 * Ideally we could impliment a balance function for each
 * insert/remove. Or we could periodically remake the tree
 * when the balance becomes too bad.
 */

namespace Structures    {
    
    template <class T>
    struct Node {
        
        Node(Float3 v = Float3(0.f, 0.f, 0.f), T t = T()) : data(t), value(v), left(NULL), right(NULL) {}
        ~Node() { delete left; delete right; }
        
        T data;
        Float3 value;
        Node<T> * left;
        Node<T> * right;
    };
    
    template <class T>
    class KDTree3D {
        
    public:
        
        KDTree3D() { root = NULL; }
        
        ~KDTree3D() { delete root; }
        
        void insertElement(Float3 element, T data) { insertElementHelper(element, data, root, 0); }
        
        void removeElement(Float3 element) { removeElementHelper(element, root, 0); }
        
        Node<T> * findNearestNeighbor(Float3 query) {
            return findNearestNeighborHelper(query, root, 0);
        }
        
    private:
        
        Node<T> * root;
        /*
         * epsilon is used to determine if two Float3's are equal.
         * infinity is used in the findMin function
         */
        float epsilon = 0.0000000001f;
        float infinity = 99999999999999;
        
        int DIM = 3;
        
        
        Float3 findMin(Node<T> * cRoot, int searchDimension, int currentDimension) {
            
            if (cRoot == NULL) {
                return Float3(infinity, infinity, infinity);
            }
            if (cRoot->left == NULL && cRoot->right == NULL) {
                //Base case
                return cRoot->value;
            }
            
            int nextDimension = (currentDimension + 1) % DIM;
            
            if (currentDimension == searchDimension) {
                //We only visit the left subtree
                
                if (cRoot->left == NULL) {
                    return cRoot->value;
                }
                else {
                    return findMin(cRoot->left, searchDimension, nextDimension);
                }
            }
            else {
                Float3 left = findMin(cRoot->left, searchDimension, nextDimension);
                Float3 right = findMin(cRoot->right, searchDimension, nextDimension);
                
                return (left[searchDimension] < right[searchDimension]) ? left : right;
            }
        }
        
        void removeElementHelper(Float3 element, Node<T> * & cRoot, int currentDimension) {
            
            if (cRoot == NULL) {
                //Our point is not found. Do nothing.
                return;
            }
            
            int nextDimension = (currentDimension + 1) % DIM;
            
            if (dist_squared(cRoot->value, element) < epsilon) {
                //This is the node we want to delete
                
                if (cRoot->right != NULL) {
                    cRoot->value = findMin(cRoot->right, currentDimension, nextDimension);
                    removeElementHelper(cRoot->value, cRoot->right, nextDimension);
                }
                else if (cRoot->left != NULL) {
                    cRoot->value = findMin(cRoot->left, currentDimension, nextDimension);
                    removeElementHelper(cRoot->value, cRoot->left, nextDimension);
                }
                else {
                    delete cRoot;
                    cRoot = NULL;
                }
            }   //else we need to search for our element
            else if (cRoot->value[currentDimension] > element[currentDimension]) {
                removeElementHelper(element, cRoot->left, nextDimension);
            }
            else {
                removeElementHelper(element, cRoot->right, nextDimension);
            }
        }
        
        void insertElementHelper(Float3 value, T data, Node<T> * & cRoot, int currentDimension) {
            
            if (cRoot == NULL) {
                cRoot = new Node<T>(value, data);
                return;
            }
            
            int nextDimension = (currentDimension + 1) % DIM;
            
            if (value[currentDimension] < cRoot->value[currentDimension]) {
                //Insert left
                insertElementHelper(value, data, cRoot->left, nextDimension);
            }
            else {
                //Insert right
                insertElementHelper(value, data, cRoot->right, nextDimension);
            }
        }
        
        Node<T> * findNearestNeighborHelper(Float3 query, Node<T> * cRoot, int currentDimension) {
            
            if (cRoot == NULL || (cRoot->left == NULL && cRoot->right == NULL)) {
                return cRoot;
            }
            
            Node<T> * currentBestNode;
            int nextDimension = (currentDimension + 1) % DIM;
            
            bool checkedLeftHyperrectangle;
            
            if (query[currentDimension] < cRoot->value[currentDimension]) {
                //Search left
                currentBestNode = findNearestNeighborHelper(query, cRoot->left, nextDimension);
                checkedLeftHyperrectangle = true;
            }
            else {
                //Search right
                currentBestNode = findNearestNeighborHelper(query, cRoot->right, nextDimension);
                checkedLeftHyperrectangle = false;
            }
            
            if (currentBestNode == NULL || dist_squared(query, cRoot->value) < dist_squared(query, currentBestNode->value)) {
                currentBestNode = cRoot;
            }
            
            float distSqrdToSplittingPlane = query[currentDimension] - cRoot->value[currentDimension];
            distSqrdToSplittingPlane *= distSqrdToSplittingPlane;
            
            if (distSqrdToSplittingPlane < dist_squared(query, currentBestNode->value)) {
                //We need to check the other hyperrectangle
                
                Node<T> * possibleBest;
                
                if (checkedLeftHyperrectangle) {
                    possibleBest = findNearestNeighborHelper(query, cRoot->right, nextDimension);
                }
                else {
                    possibleBest = findNearestNeighborHelper(query, cRoot->left, nextDimension);
                }
                
                if (possibleBest != NULL && dist_squared(query, possibleBest->value) < dist_squared(query, currentBestNode->value)) {
                    currentBestNode = possibleBest;
                }
            }
            
            return currentBestNode;

        }
    };
}

#endif