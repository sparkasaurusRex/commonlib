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
    
    struct Node {
        
        Node(Float3 v = Float3(0.f, 0.f, 0.f)) : value(v), left(NULL), right(NULL) {}
        ~Node() { delete left; delete right; }
        
        Float3 value;
        Node * left;
        Node * right;
    };
    
    class KDTree3D {
        
    public:
        
        KDTree3D() { root = NULL; }
        
        ~KDTree3D() { delete root; }
        
        void insertElement(Float3 element) { insertElementHelper(element, root, 0); }
        
        void removeElement(Float3 element) { removeElementHelper(element, root, 0); }
        
        Float3 findNearestNeighbor(Float3 query) {
            Node * result = findNearestNeighborHelper(query, root, 0);
            return (result == NULL) ? Float3(-69, -69, -69) : result->value;
        }
        
    private:
        
        Node * root;
        
        Float3 findMin(Node * cRoot, int searchDimension, int currentDimension);
        
        void removeElementHelper(Float3 element, Node * & cRoot, int currentDimension);
        
        void insertElementHelper(Float3 value, Node * & cRoot, int currentDimension);
        
        Node * findNearestNeighborHelper(Float3 query, Node * cRoot, int currentDimension);
        
        /*
         * epsilon is used to determine if two Float3's are equal.
         * infinity is used in the findMin function
         */
        float epsilon = 0.0000000001f;
        float infinity = 99999999999999;
        
        int DIM = 3;
    };
}

#endif