#include "kdtree.h"

using namespace Math;
using namespace Structures;

void KDTree3D::removeElementHelper(Float3 element, Node * & cRoot, int currentDimension) {

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

Float3 KDTree3D::findMin(Node * cRoot, int searchDimension, int currentDimension) {
    
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

void KDTree3D::insertElementHelper(Float3 value, Node * & cRoot, int currentDimension) {
    
    if (cRoot == NULL) {
        cRoot = new Node(value);
        return;
    }
    
    int nextDimension = (currentDimension + 1) % DIM;
    
    if (value[currentDimension] < cRoot->value[currentDimension]) {
        //Insert left
        insertElementHelper(value, cRoot->left, nextDimension);
    }
    else {
        //Insert right
        insertElementHelper(value, cRoot->right, nextDimension);
    }
}

Node * KDTree3D::findNearestNeighborHelper(Float3 query, Node * cRoot, int currentDimension) {
    
    if (cRoot == NULL || (cRoot->left == NULL && cRoot->right == NULL)) {
        return cRoot;
    }
    
    Node * currentBestNode;
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
        
        Node * possibleBest;
        
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
