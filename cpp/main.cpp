#include "biplanarTester.h"
#include "candidateBuilder.h"
#include "biplanarSAT.h"

int main() {
    int m = 4, t = 3;
    int n = m * ((1 << (t-1)) - 1) + 2;
    vector<Edge> edges = cycleStackEdge(m,t);
    printEdges(edges);
    // int a = 4, b = 4;
    // n = a*b + 1;
    // vector<Edge> edges = triangularGridMaxPlanarGraphEdge(a,b);
    // printEdges(edges);
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

