#include "biplanarTester.h"
#include "candidateBuilder.h"
#include "SATBiplanarTester.h"

int main() {
    int n1 = 9;
    int n = 2*n1;
    maxEdges = 3 * n - 6;
    // wheel graph with two middle vertices
    auto* edgesFirst = wheelGraphEdge(n1, 2);
    auto* edges = blowup(edgesFirst, n1);
    preprocessEdges(edges, n);
    delete edgesFirst;
    if (isBiplanarSAT(*edges, n)) {
        cout << "it is biplanar" << endl;
    } else {
        cout << "it is NOT biplanar" << endl;
    }
    delete edges;
    return 0;
}

