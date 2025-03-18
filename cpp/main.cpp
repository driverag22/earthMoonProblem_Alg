#include "biplanarTester.h"
#include "candidateBuilder.h"
#include "biplanarSAT.h"

int main() {
    /* vector<Edge>* edges = triangularGridMaxPlanarGraphEdge(4, 4); */
    /* edges = blowup(edges, 16); */
    int n = 8;
    vector<Edge> edges = completeGraphEdge(8);
    Graph g = constructGraph(edges, 8);
    if (isBiplanarSAT(edges, 8)) {
        cout << "planarrr" << endl;
    }

    /* if (isBiplanarSAT(3, *completeGraphEdge(3))) { */
    /*     cout << "biplanar!" << endl; */
    /* } */
    /* for (int n = 11; n < 13; n++) { */
    /*     Graph g = buildMaximalPlanarGraph(n); */
    /*     vector<Edge>* edges = extractEdges(g); */
    /*     edges = blowup(edges, n); */
    /*     g = constructGraph(*edges, n); */
    /*     outputGraph(g, "bMaxPlanar" + to_string(n)); */
    /*     delete edges; */

    /*     vector<Edge>* edges2 = maximalPlanarGraphEdge(n); */
    /*     edges2 = blowup(edges2, n); */
    /*     g = constructGraph(*edges2, n); */
    /*     outputGraph(g, "maxPlanar" + to_string(n)); */
    /*     delete edges2; */
    /* } */
    // wheel graph with two middle vertices
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    /* computeCandidateGraphs(20, 80, 100000, true, true); */
    return 0;
}

