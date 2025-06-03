#include "biplanarTester.h"
int maxEdges;

/// Preprocess edge list to order by edge difficulty.
void preprocessEdges(vector<Edge>* edges, int n) {
    vector<int> degree(n, 0);

    for (const auto& e : *edges) {
        degree[e.first]++;
        degree[e.second]++;
    }
    
    sort(edges->begin(), edges->end(), [&degree](const Edge& a, const Edge& b) {
        return (degree[a.first] + degree[a.second]) > (degree[b.first] + degree[b.second]);
    });
}

/// Back-tracking recursive implementation of biplanarity checker.
bool isBiplanar(vector<Edge>& edges, int index, Graph& g1, Graph& g2) {
    if ((size_t)index == edges.size()) {
        if (isPlanar(g1) && isPlanar(g2)) {
            cout << "Graph is biplanar!" << endl;
            cout << "Number of edges:" << endl;
            cout << "    |E(G1)| = " << num_edges(g1) << endl;
            cout << "    |E(G2)| = " << num_edges(g2) << endl;
            cout << "    |E(G1)| + |E(G2)| = " << num_edges(g1) + num_edges(g2) << endl;
            cout << "    should be = " << edges.size() << endl;
            outputPartitions(g1, g2);
            return true;
        }
        return false;
    }
    
    // check edge bounds
    if (num_edges(g1) > (size_t)maxEdges || num_edges(g2) > (size_t)maxEdges)
        return false;

    // check still planar
    if (!isPlanar(g1) || !isPlanar(g2))
        return false;
    
    // go for balanced partitions first
    if (num_edges(g1) <= num_edges(g2)) {
        add_edge(edges[index].first, edges[index].second, g1);
        if (isBiplanar(edges, index + 1, g1, g2))
            return true;
        remove_edge(edges[index].first, edges[index].second, g1);

        add_edge(edges[index].first, edges[index].second, g2);
        if (isBiplanar(edges, index + 1, g1, g2))
            return true;
        remove_edge(edges[index].first, edges[index].second, g2);
    } else {
        add_edge(edges[index].first, edges[index].second, g2);
        if (isBiplanar(edges, index + 1, g1, g2))
            return true;
        remove_edge(edges[index].first, edges[index].second, g2);

        add_edge(edges[index].first, edges[index].second, g1);
        if (isBiplanar(edges, index + 1, g1, g2))
            return true;
        remove_edge(edges[index].first, edges[index].second, g1);
    }
    
    return false;
}

void testBiplanarity(vector<Edge>* edges, int n) {
    maxEdges = 3 * n - 6;
    if ((edges->size()) > (size_t)2*maxEdges) {
        cout << "Graph cannot be biplanar, 6n-12 = " 
            << 2*maxEdges << " but number of edges is "
            << edges->size() << endl;
        delete edges;
        return;
    }
    preprocessEdges(edges, n);
    cout << "6n-12 = " << 2*maxEdges << " and number of edges is "
        << edges->size() << endl;

    Graph g1(n), g2(n);
    // add first edge arbitrarily, it doesn't matter due to symmetry
    add_edge((*edges)[0].first, (*edges)[0].second, g1);
    if (!isBiplanar(*edges, 1, g1, g2)) {
        cout << "Graph is not biplanar." << endl;
    }
}
