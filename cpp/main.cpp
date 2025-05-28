#include "candidateBuilder.h"
#include "biplanarSAT.h"

int main() {
    ////////////////
    // SAT solver //
    ////////////////
    // int a = 5, b = 4, n = a * b;
    // vector<Edge> edges = strongProductEdge(pathGraphEdge(a), a, completeGraphEdge(b), b);
    // auto start = chrono::high_resolution_clock::now();
    // if (isBiplanarSAT(edges, n)) {
    //     cout << "biplanar" << endl;
    // }
    // auto end = chrono::high_resolution_clock::now();
    // chrono::duration<double> elapsed = end - start;
    // cout << "Took: " << elapsed.count() << " seconds.\n";

    // int n = 8;
    // vector<Edge> edges = completeGraphEdge(n);
    // auto start = chrono::high_resolution_clock::now();
    // if (isBiplanarSAT(edges, n)) {
    //     cout << "biplanar" << endl;
    // }
    // auto end = chrono::high_resolution_clock::now();
    // chrono::duration<double> elapsed = end - start;
    // cout << "Took: " << elapsed.count() << " seconds.\n";

    //////////////////////
    // Candidate finder //
    //////////////////////
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    // computeCandidateGraphs(20, 80, 100000, true, true);

    ////////////////////////
    // Blow-up of biwheel //
    ////////////////////////
    // int m = 5;
    // int n = m+2; // spokes: 5,6
    // vector<Edge> edges = wheelGraphEdge(n, 2);
    // printEdges(edges);
    // cout << endl;
    // edges = blowup(edges, n);
    // printEdges(edges);

    return 0;
}

