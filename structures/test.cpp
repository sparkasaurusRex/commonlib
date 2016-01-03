#include <iostream>
#include <vector>

#include "kdtree.h"

using namespace Structures;
using namespace std;
using namespace Math;

int main(int argc, char **argv) {

    int numPoints = 10;
    if (argc > 1) {
        numPoints = atoi(argv[1]);
    }

    srand(time(NULL));

    KDTree3D<int> tree;

    Float3 node(0.5f, 0.6f, -0.9f);

    Float3 nearest(1000, 1000, 1000);

    for (int i = 0; i < numPoints; i++) {

        Float3 element(random(-1.f, 1.f), random(-1.f, 1.f), random(-1.f, 1.f));

        if (dist_squared(node, element) < dist_squared(node, nearest)) {
            nearest = element;
        }

        tree.insertElement(element, i);
    }

    cout << "Node: " << node << endl;
    cout << "Actual nearest: " << nearest << endl;
    cout << "KDTree nearest: " << tree.findNearestNeighbor(node)->value << endl;

    for (int i = 0; i < numPoints / 2; i++) {
        tree.removeElement(tree.findNearestNeighbor(node)->value);
    }

    tree.insertElement(nearest, 0);

    cout << "KDTree nearest: " << tree.findNearestNeighbor(node)->value << endl;

    return 0;
}
