#include "biplanarTester.h"
int maxEdges;

/// Returns whether given graph is planar.
bool isPlanar(Graph& g) {
    return boyer_myrvold_planarity_test(g);
}

/// Helper function to output edge partitions.
void outputPartitions(const vector<Edge>& partition1, const vector<Edge>& partition2) {
    ofstream file("data/partitions.txt");
    if (!file) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    for (const auto& e : partition1) file << e.first << " " << e.second << "\n";
    file << "\n";
    for (const auto& e : partition2) file << e.first << " " << e.second << "\n";

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
/// Returns the strong product of two graphs.
vector<Edge>* strongProduct(const vector<Edge>* graph1, int n1, const vector<Edge>* graph2, int n2) {
    auto* productEdges = new vector<Edge>();
    
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < n2; ++j) {
            // Connect based on edges in graph1
            for (const auto& e : *graph1) {
                productEdges->push_back({e.first * n2 + j, e.second * n2 + j});
            }
            
            // Connect based on edges in graph2
            for (const auto& e : *graph2) {
                productEdges->push_back({i * n2 + e.first, i * n2 + e.second});
            }
            
            // Strong product cross edges
            if (i + 1 < n1) {
                for (int k = 0; k < n2; ++k) {
                    productEdges->push_back({i * n2 + j, (i + 1) * n2 + k});
                }
            }
        }
    }
    return productEdges;
}
//////

/// Preprocess edge list to order by edge difficulty.
vector<Edge> preprocessEdges(const vector<Edge>& edges, int n) {
    vector<int> degree(n, 0);
    for (const auto& e : edges) {
        degree[e.first]++;
        degree[e.second]++;
    }
    
    sort(edges.begin(), edges.end(), [&degree](const Edge& a, const Edge& b) {
        return max(degree[a.first], degree[a.second]) > max(degree[b.first], degree[b.second]);
    });

    return edges;
}

/// Back-tracking recursive implementation of biplanarity checker.
bool isBiplanar(vector<Edge>& edges, int index, Graph& g1, Graph& g2, vector<Edge>& partition1, vector<Edge>& partition2) {
    if ((size_t)index == edges.size()) {
        if (isPlanar(g1) && isPlanar(g2)) {
            cout << "Graph is biplanar!" << endl;
            outputPartitions(partition1, partition2);
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
    partition1.push_back(edges[index]);
    if (isBiplanar(edges, index + 1, g1, g2, partition1, partition2))
        return true;
    partition1.pop_back();
    remove_edge(edges[index].first, edges[index].second, g1);
    
    add_edge(edges[index].first, edges[index].second, g2);
    partition2.push_back(edges[index]);
    if (isBiplanar(edges, index + 1, g1, g2, partition1, partition2))
        return true;
    partition2.pop_back();
    remove_edge(edges[index].first, edges[index].second, g2);
    
    return false;
}
