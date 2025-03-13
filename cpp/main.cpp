#include "biplanarTester.h"
#include "candidateBuilder.h"

int main() {
    for (int n = 5; n < 11; n++) {
        Graph g = buildMaximalPlanarGraph(n);
        vector<Edge>* edges = extractEdges(g);
        edges = blowup(edges, n);
        g = constructGraph(*edges, n);
        outputGraph(g, "bMaxPlanar" + to_string(n));
    }
    // wheel graph with two middle vertices
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    /* computeCandidateGraphs(20, 80, 100000, true, true); */
    return 0;
}

