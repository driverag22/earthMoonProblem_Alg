#include "candidateBuilder.h"
#include "biplanarSAT.h"
#include "biplanarTester.h"

int main() {
    /////////////////////////
    // Backtracking vs SAT //
    /////////////////////////
    int n; vector<Edge> edges;

    int a = 5, b = 4;
    n = a * b;
    edges = strongProductEdge(pathGraphEdge(a), a, completeGraphEdge(b), b);
    removeVertexEdges(edges, 0);

    auto start = chrono::high_resolution_clock::now();
    testBiplanarity(&edges, n);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Took: " << elapsed.count() << " seconds.\n";

    start = chrono::high_resolution_clock::now();
    if (isBiplanarSAT(edges, n)) {
        cout << "biplanar" << endl;
    }
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed2 = end - start;
    cout << "Took: " << elapsed2.count() << " seconds.\n";

    // n = 8;
    // edges = completeGraphEdge(n);

    // auto start = chrono::high_resolution_clock::now();
    // testBiplanarity(&edges, n);
    // auto end = chrono::high_resolution_clock::now();
    // chrono::duration<double> elapsed = end - start;
    // cout << "Took: " << elapsed.count() << " seconds.\n";

    // start = chrono::high_resolution_clock::now();
    // if (isBiplanarSAT(edges, n)) {
    //     cout << "biplanar" << endl;
    // }
    // end = chrono::high_resolution_clock::now();
    // chrono::duration<double> elapsed2 = end - start;
    // cout << "Took: " << elapsed2.count() << " seconds.\n";

    //////////////////////
    // Candidate finder //
    //////////////////////
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    // computeCandidateGraphs(20, 80, 100000, true, true);

    ////////////////////////
    // Blow-up of biwheel //
    ////////////////////////
    // int m = 5;
    // n = m+2; // spokes: 5,6
    // edges = wheelGraphEdge(n, 2);
    // printEdges(edges);
    // cout << endl;
    // edges = blowup(edges, n);
    // printEdges(edges);

    return 0;
}

