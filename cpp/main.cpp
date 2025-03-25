#include "biplanarTester.h"
#include "candidateBuilder.h"
#include "biplanarSAT.h"

int main() {
    int n = 30;
    int num_vertices = 2*n + 2;
    vector<Edge> edges = k4stack(n);
    edges = blowup(edges, num_vertices);
    Graph g = constructGraph(edges);
    outputGraph(g);
    auto start = chrono::high_resolution_clock::now();
    if (isBiplanarSAT(edges, num_vertices)) {
        cout << "biplanarrr" << endl;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Took: " << elapsed.count() << " seconds.\n";

    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    /* computeCandidateGraphs(20, 80, 100000, true, true); */
    return 0;
}

