#include "biplanarTester.h"

int main() {
    // length of path, size of complete graphs
    int n1 = 3, n2 = 4;
    int n = n1 * n2;
    maxEdges = 3 * n - 6;
    
    auto* edges = strongProduct(pathGraph(n1), n1, completeGraph(n2), n2);
    edges = preprocessEdges(edges, n);
    Graph g1(n), g2(n);
    vector<Edge> partition1, partition2;
    
    if (!isBiplanar(*edges, 0, g1, g2, partition1, partition2)) {
        cout << "Graph is not biplanar." << endl;
    }
    
    delete edges;
    return 0;
}
