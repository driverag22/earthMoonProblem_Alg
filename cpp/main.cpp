#include "biplanarTester.h"
#include "candidateBuilder.h"
#include "biplanarSAT.h"

int main() {
    int cols = 4, rows = 4;
    int n = rows * cols;
    vector<Edge> edges = triangularGridMaxPlanarGraphEdge(cols, rows);
    printEdges(edges);
    edges = blowup(edges, n);
    cout << endl;
    printEdges(edges);
    /* auto start = chrono::high_resolution_clock::now(); */
    /* if (isBiplanarSAT(edges, n)) { */
    /*     cout << "biplanarrr" << endl; */
    /* } */
    /* auto end = chrono::high_resolution_clock::now(); */
    /* chrono::duration<double> elapsed = end - start; */
    /* cout << "Took: " << elapsed.count() << " seconds.\n"; */
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    /* computeCandidateGraphs(20, 80, 100000, true, true); */
    return 0;
}

