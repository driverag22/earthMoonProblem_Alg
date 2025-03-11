#include "biplanarTester.h"
#include "candidateBuilder.h"

int main() {
    int n1 = 3, n2 = 4;
    int n = n1*n2;
    maxEdges = 3 * n - 6;
    // wheel grah with two middle vertices
    auto* edges = strongProductEdge(pathGraphEdge(n1), n1, completeGraphEdge(n2), n2);
    /* delete edgesFirst; */
    /* preprocessEdges(edges, n); */
    printEdges(*edges);
    /* testBiplanarity(edges, n); */
    /* delete edges; */
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    /* computeCandidateGraphs(20, 80, 100000, true, true); */
    return 0;
}

