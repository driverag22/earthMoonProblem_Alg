#include "biplanarTester.h"
#include "candidateBuilder.h"

int main() {
    /* int n1 = 9; */
    /* int n = 2*n1; */
    /* maxEdges = 3 * n - 6; */
    /* // wheel graph with two middle vertices */
    /* auto* edgesFirst = wheelGraphEdge(n1, 2); */
    /* auto* edges = blowup(edgesFirst, n1); */
    /* delete edgesFirst; */
    /* preprocessEdges(edges, n); */
    /* printEdges(*edges); */
    /* testBiplanarity(edges, n); */
    /* delete edges; */
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    computeCandidateGraphs(20, 80, 100000, true, true);
    return 0;
}

