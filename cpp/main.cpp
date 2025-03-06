#include "biplanarTester.h"

int main() {
    int n1 = 7;
    int n = 2*n1;
    maxEdges = 3 * n - 6;
    // wheel graph with two middle vertices
    auto* edgesFirst = wheelGraphEdge(n1, 2);
    printEdges(*edgesFirst);
    auto* edges = blowup(edgesFirst, n1);
    cout << endl;
    printEdges(*edges);

    testBiplanarity(edges, n);
    
    delete edges;
    return 0;
}

// (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
/* computeCandidateGraphs(10, 100, 1, true, true); */
