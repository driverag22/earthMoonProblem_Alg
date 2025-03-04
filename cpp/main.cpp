#include "triplanarTester.h"

int main() {
    // length of cycle/path, size of complete graphs
    int n1 = 5, n2 = 10;
    int n = n1 * n2;
    maxEdges = 3 * n - 6;
    
    auto* edges = strongProduct(cycleGraph(n1), n1, completeGraph(n2), n2);
    // K_6: (0-9) -> (0-5)
    removeVertexEdges(edges, 6);
    removeVertexEdges(edges, 7);
    removeVertexEdges(edges, 8);
    removeVertexEdges(edges, 9);
    // K_1: (20-29) -> (20)
    removeVertexEdges(edges, 21);
    removeVertexEdges(edges, 22);
    removeVertexEdges(edges, 23);
    removeVertexEdges(edges, 24);
    removeVertexEdges(edges, 25);
    removeVertexEdges(edges, 26);
    removeVertexEdges(edges, 27);
    removeVertexEdges(edges, 28);
    removeVertexEdges(edges, 29);
    // K_6: (40-49) -> (40-45)
    removeVertexEdges(edges, 46);
    removeVertexEdges(edges, 47);
    removeVertexEdges(edges, 48);
    removeVertexEdges(edges, 49);
    // check edge bound before starting
    printEdges(*edges);
    if ((edges->size()) > (size_t)3*maxEdges) {
        cout << "Graph cannot be biplanar, 3n-18 = " 
            << 3*maxEdges << " but number of edges is "
            << edges->size() << endl;
        delete edges;
        return 0;
    }
    preprocessEdges(edges, n);
    // print stuff for testing
    cout << "9n-18 = " << 3*maxEdges << " and number of edges is "
        << edges->size() << endl;

    Graph g1(n), g2(n), g3(n);
    // add first edge arbitrarily, it doesn't matter due to symmetry
    add_edge((*edges)[0].first, (*edges)[0].second, g1);
    if (!isTriplanar(*edges, 1, g1, g2, g3)) {
        cout << "Graph is not biplanar." << endl;
    }
    
    delete edges;
    return 0;
}
