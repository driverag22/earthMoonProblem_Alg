#include "biplanarTester.h"
#include "candidateBuilder.h"
#include "biplanarSAT.h"

int main() {
    int a = 4, b = 4;
    int n = a*b + 1;
    // vector<Edge> edges = triangularGridMaxPlanarGraphEdge(a,b);
    vector<Edge> edges = {
        pair(0, 1),  
        pair(0, 2),
        pair(1, 2),
        pair(1, 3),
        pair(2, 3),
        pair(0, 3),
        pair(0, 4),
        pair(1, 4),
        pair(2, 4),
        pair(1, 5),
        pair(2, 5),
        pair(3, 5),
        pair(0, 6),
        pair(1, 6),
        pair(3, 6),
    };
    // int n = 4;
    // vector<Edge> edges = cycleGraphEdge(n);
    printEdges(edges);
    cout << endl;
    edges = blowup(edges, 4);
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

