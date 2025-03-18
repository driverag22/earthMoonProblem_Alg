#include "biplanarTester.h"
#include "candidateBuilder.h"
#include "biplanarSAT.h"

int main() {
    /* vector<Edge>* edges = triangularGridMaxPlanarGraphEdge(4, 4); */
    int n1 = 5, n2 = 4;
    int n = n1 * n2 - 1;
    vector<Edge> edges = strongProductEdge(pathGraphEdge(n1), n1, 
                                           completeGraphEdge(n2), n2);
    removeVertexEdges(edges, 19);
    edges.emplace_back(0,18);
    edges.emplace_back(1,17);
    edges.emplace_back(2,16);
    edges.emplace_back(3,15);
    Graph g = constructGraph(edges, n);

    auto start = chrono::high_resolution_clock::now();
    if (isBiplanarSAT(edges, n)) {
        cout << "biplanarrr" << endl;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Took: " << elapsed.count() << " seconds.\n";

    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    /* computeCandidateGraphs(20, 80, 100000, true, true); */
    return 0;
}

