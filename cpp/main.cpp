#include "biplanarTester.h"
#include "candidateBuilder.h"
#include "biplanarSAT.h"

int main() {
    // int a = 3, b = 3;
    // int n = a*b + 1;
    // vector<Edge> edges = triangularGridMaxPlanarGraphEdge(a,b);
    int n = 4;
    vector<Edge> edges = cycleGraphEdge(n);
    printEdges(edges);
    cout << endl;
    edges = blowup(edges, n);
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

