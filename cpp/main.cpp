#include "biplanarTester.h"
#include "candidateBuilder.h"

int main() {
    int n = 15;
    vector<Edge>* edges = maximalPlanarGraphEdge(n);
    edges = blowup(edges, n);
    Graph g = constructGraph(*edges, n);
    outputGraph(g, "candidateDisprove");
    // wheel graph with two middle vertices
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    /* computeCandidateGraphs(20, 80, 100000, true, true); */
    return 0;
}

