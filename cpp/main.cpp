#include "biplanarTester.h"
#include "candidateBuilder.h"

int main() {
    printProgressBar(0, 6, 
                     "Iteration n = 9");
    for (int n1 = 9; n1 <= 15; n1++) {
        int n = 2*n1;
        maxEdges = 3 * n - 6;
        // wheel graph with two middle vertices
        auto* edgesFirst = wheelGraphEdge(n1, 2);
        /* printEdges(*edgesFirst); */
        auto* edges = blowup(edgesFirst, n1);
        /* cout << endl; */
        /* printEdges(*edges); */
        testBiplanarity(edges, n);
        printProgressBar(n1-9, 6, "Iteration n = " + to_string(n1));
        delete edgesFirst;
        delete edges;
    }
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    /* computeCandidateGraphs(20, 20, 1000000, true, true); */
    return 0;
}

