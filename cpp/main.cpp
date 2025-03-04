#include "biplanarTester.h"

int main() {
    // length of cycle/path, size of complete graphs
    int n1 = 5, n2 = 4;
    int n = n1 * n2;
    maxEdges = 3 * n - 6;
    
    auto* edges = strongProduct(pathGraph(n1), n1, completeGraph(n2), n2);
    removeVertexEdges(edges, 4);
    edges->push_back({0,16});
    edges->push_back({1,17});
    edges->push_back({2,18});
    edges->push_back({3,19});
    // check edge bound before starting
    if ((edges->size()) > (size_t)2*maxEdges) {
        cout << "Graph cannot be biplanar, 6n-12 = " 
            << 2*maxEdges << " but number of edges is "
            << edges->size() << endl;
        delete edges;
        return 0;
    }
    preprocessEdges(edges, n);
    // print stuff for testing
    printEdges(*edges);
    cout << "6n-12 = " << 2*maxEdges << " and number of edges is "
        << edges->size() << endl;

    Graph g1(n), g2(n);
    // add first edge arbitrarily, it doesn't matter due to symmetry
    add_edge((*edges)[0].first, (*edges)[0].second, g1);
    if (!isBiplanar(*edges, 1, g1, g2, "")) {
        cout << "Graph is not biplanar." << endl;
    }
    
    delete edges;
    return 0;
}
