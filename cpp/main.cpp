#include "biplanarTester.h"
#include "candidateBuilder.h"

int main() {
    int n = 15;
    Graph g = maximalPlanarGraph(n);
    if (isPlanar(g)) {
        cout << "yes" << endl;
        outputGraph(g, "candidateDisprove");
    } else {
        cout << "no" << endl;
    }
    // wheel graph with two middle vertices
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    /* computeCandidateGraphs(20, 80, 100000, true, true); */
    return 0;
}

