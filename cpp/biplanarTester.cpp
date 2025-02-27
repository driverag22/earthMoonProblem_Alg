#include "biplanarTester.h"
int maxEdges;

/// Returns whether given graph is planar.
bool isPlanar(Graph& g) {
    return boyer_myrvold_planarity_test(g);
}

/// Helper function to output edge partitions.
/* void outputPartitions(const vector<Edge>& partition1, const vector<Edge>& partition2) { */
void outputPartitions(Graph& g1, Graph& g2) {
    ofstream file("data/partitions.txt");
    if (!file) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    for (const auto& e : make_iterator_range(edges(g1))) file << source(e, g1) << " " << target(e, g1) << "\n";
    file << "\n";
    for (const auto& e : make_iterator_range(edges(g2))) file << source(e, g2) << " " << target(e, g2) << "\n";

    file.close();
    cout << "Partitions saved to partitions.txt" << endl;
}

/// Prints edges of a given graph (for debugging purposes).
void printEdges(const vector<Edge>& edges) {
    for (const auto& e : edges) {
        cout << "(" << e.first << ", " << e.second << ")\n";
    }
}

////// Graph constructors
/// Returns path edge-set on [numVertices] vertices.
vector<Edge>* pathGraph(int numVertices) {
    auto* edges = new vector<Edge>();
    for (int i = 0; i < numVertices - 1; ++i) {
        edges->push_back({i, i + 1});
    }
    return edges;
}

/// Returns cycle edge-set on [numVertices] vertices.
vector<Edge>* cycleGraph(int numVertices) {
    auto* edges = new vector<Edge>();
    for (int i = 0; i < numVertices - 1; ++i) {
        edges->push_back({i, i + 1});
    }
    edges->push_back({0, numVertices - 1});
    return edges;
}

/// Returns completeGraph edge-set on [numVertices] vertices.
vector<Edge>* completeGraph(int numVertices) {
    auto* edges = new vector<Edge>();
    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            edges->push_back({i, j});
        }
    }
    return edges;
}
//////

////// Graph operations
/// Removes all edges connected to a specific vertex.
void removeVertexEdges(vector<Edge>& edges, int vertex) {
    edges.erase(remove_if(edges.begin(), edges.end(), 
                [vertex](const Edge& e) { return e.first == vertex || e.second == vertex; }),
                edges.end());
}

/// Returns the strong product of two graphs.
vector<Edge>* strongProduct(const vector<Edge>* graph1, int n1, const vector<Edge>* graph2, int n2) {
    // create adjacency matrices for G1 and G2 
    // to quickly check if (u,v) is an edge of either
    vector<vector<bool>> adj1(n1, vector<bool>(n1, false));
    vector<vector<bool>> adj2(n2, vector<bool>(n2, false));
    for (const Edge& e : *graph1) {
        int u = e.first, v = e.second;
        adj1[u][v] = true;
        adj1[v][u] = true;
    }
    for (const Edge& e : *graph2) {
        int u = e.first, v = e.second;
        adj2[u][v] = true;
        adj2[v][u] = true;
    }
    
    vector<Edge>* productEdges = new vector<Edge>();
    // we store the cartesian product in a 1D array
    // flattening: (a,b) in V(G1)*V(G2) = a * n2 + b

    // go over all possible vertices
    for (int u = 0; u < n1; ++u) {
        for (int v = 0; v < n2; ++v) {
            int index1 = u * n2 + v; // index1 \cong (u, v)

            // we add edge iff ( (u = u2 or u ~ u2) and (v = v2 or v ~ v2)
            for (int u2 = 0; u2 < n1; ++u2) {
                if (u != u2 && !adj1[u][u2]) // skip if not(u = u2 or u ~ u2)
                    continue;
                for (int v2 = 0; v2 < n2; ++v2) {
                    if (v != v2 && !adj2[v][v2]) // skip if not(v = v2 or v ~ v2)
                        continue;

                    int index2 = u2 * n2 + v2; // index2 \cong (u2, v2)

                    if (index1 < index2) {
                        productEdges->push_back(make_pair(index1, index2));
                    }
                }
            }
        }
    }
    
    return productEdges;
}
//////

/// Preprocess edge list to order by edge difficulty.
void preprocessEdges(vector<Edge>& edges, int n) {
    vector<int> degree(n, 0);

    for (const auto& e : edges) {
        degree[e.first]++;
        degree[e.second]++;
    }
    
    sort(edges.begin(), edges.end(), [&degree](const Edge& a, const Edge& b) {
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
    
    add_edge(edges[index].first, edges[index].second, g1);
    if (isBiplanar(edges, index + 1, g1, g2))
        return true;
    remove_edge(edges[index].first, edges[index].second, g1);

    add_edge(edges[index].first, edges[index].second, g2);
    if (isBiplanar(edges, index + 1, g1, g2))
        return true;
    remove_edge(edges[index].first, edges[index].second, g2);
    
    return false;
}
