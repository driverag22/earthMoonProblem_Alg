#include "helperFunctions.h"

/// Returns graph corresponding to given edge list.
Graph constructGraph(vector<Edge>& edges, int numVerices) {
    Graph G(numVerices);
    for (const auto& e : edges)
        add_edge(e.first, e.second, G);
    return G;
}

/// Returns whether given graph is planar.
bool isPlanar(Graph& g) {
    return boyer_myrvold_planarity_test(g);
}

/// Helper function to output graph.
void outputGraph(Graph& g, string filename) {
    ofstream file("data/" + filename + ".txt");
    if (!file) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    for (const auto& e : make_iterator_range(edges(g))) file << source(e, g) << " " << target(e, g) << "\n";

    file.close();
}

/// Helper function to output edge partitions.
void outputPartitions(Graph& g1, Graph& g2, string filename) {
    ofstream file("data/" + filename + ".txt");
    if (!file) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    for (const auto& e : make_iterator_range(edges(g1))) file << source(e, g1) << " " << target(e, g1) << "\n";
    file << "\n";
    for (const auto& e : make_iterator_range(edges(g2))) file << source(e, g2) << " " << target(e, g2) << "\n";

    file.close();
}

/// Helper function to output edge partitions.
void printGraph(Graph& g) {
    for (const auto& e : make_iterator_range(edges(g))) cout << source(e, g) << " " << target(e, g) << "\n";
}

/// Prints edges of a given graph (for debugging purposes).
void printEdges(const vector<Edge>& edges) {
    for (const auto& e : edges) {
        cout << e.first << " " << e.second << endl;
    }
}

////// Graph computations
/// Recursively determines if [g] can be colored with [nColors] colors.
/// Returns true if finding a coloring with [k-1] colors takes over 
/// [time_limit_ms] ms and [timed] is true.
bool canColor(const Graph& g, int nColors, vector<int>& colors, int index, bool timed,
              const chrono::steady_clock::time_point& start_time, chrono::milliseconds time_limit) {
    int n = num_vertices(g);
    if (index == n)
        return true;

    if (timed && chrono::steady_clock::now() - start_time > time_limit)
        return false; 

    // try all colors from 0 to maxColors-1 for vertex index,
    // starting with the least used
    for (int c = 0; c < nColors; c++) {
        bool conflict = false;
        // check for conflicts w/all neighbors
        for (const auto& neighbor : make_iterator_range(adjacent_vertices(index, g))) {
            if (colors[neighbor] == c) {
                conflict = true;
                break;
            }
        }
        if (!conflict) {
            colors[index] = c;
            if (canColor(g, nColors, colors, index + 1, timed, start_time, time_limit)) return true;
            colors[index] = -1;
        }
    }
    return false;
}

/// Returns if the chromatic number of graph [g] is at least [k],
/// or if finding a coloring with [k-1] colors takes over [time_limit_s] s 
/// and [timed] is true.
bool chromaticNumberAtLeast(Graph& g, int k, bool timed, int time_limit_s) {
    int n = num_vertices(g);
    vector<int> colors(n, -1);

    auto start_time = chrono::steady_clock::now();
    chrono::milliseconds time_limit(1000 * time_limit_s);

    // if we can color g with k-1 colors, \chi(g) < k, so we return false.
    return !canColor(g, k-1, colors, 0, timed, start_time, time_limit);
}

/// Determines if there is an independent set in [g] of size [k]
/// containing the vertices in independentSet (backtracking).
bool findIndependentSet(Graph& g, vector<int> independentSet, int index, int k) {
    // if set is full-size, return true
    if (independentSet.size() == (size_t)k) return true;
    // if not, and no more vertices left, return false
    else if ((size_t)index >= num_vertices(g)) return false;

    // try excluding current vertex
    if (findIndependentSet(g, independentSet, index + 1, k)) return true;

    // check if we can add it
    for (int v : independentSet) {
        if (edge(index, v, g).second) return false;
    }
    // try adding it if we can
    independentSet.push_back(index);
    if (findIndependentSet(g, independentSet, index + 1, k)) return true;
    independentSet.pop_back();

    return false;
}

/// Checks if the independence number of [g] is at most [k].
///   - returns [\alpha(G) \leq k] == ![\alpha(G) \geq k+1]
bool independenceNumberAtMost(Graph& g, int k) {
    vector<int> indeptSet;
    return !findIndependentSet(g, indeptSet, 0, k+1);
}
//////

////// Graph constructors
/// Returns path graph on [numVertices] vertices.
Graph pathGraph(int numVertices) {
    Graph G(numVertices);
    for (int i = 0; i < numVertices - 1; ++i) {
        add_edge(i, i + 1, G);
    }
    return G;
}

/// Returns cycle graph on [numVertices] vertices.
Graph cycleGraph(int numVertices) {
    Graph G(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        int j = (i + 1) % numVertices;
        add_edge(i, j, G);
    }
    return G;
}

/// Returns complete graph on [numVertices] vertices.
Graph completeGraph(int numVertices) {
    Graph G(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            add_edge(i, j, G);
        }
    }
    return G;
}

/// Returns a maximal planar graph with [numVertices] vertices.
Graph maximalPlanarGraph(int numVertices) {
    // initial triangle
    vector<Edge> edges = { {0, 1}, {1, 2}, {2, 0} };
    
    // Add vertices one by one
    for (int i = 3; i < numVertices; i++) {
        int u1 = (i - 1) % i;   // last vertex
        int u2 = (i - 2) % i;   // second-last vertex
        int u3 = (i - 3) % i;   // third-last vertex

        // Connect new vertex to all three vertices forming the triangle
        edges.push_back({i, u1});
        edges.push_back({i, u2});
        edges.push_back({i, u3});
    }

    Graph G(numVertices);
    for (const auto& e : edges) {
        add_edge(e.first, e.second, G);
    }

    return G;
}

/// Returns path edge-set on [numVertices] vertices.
vector<Edge>* pathGraphEdge(int numVertices) {
    auto* edges = new vector<Edge>();
    for (int i = 0; i < numVertices - 1; ++i) {
        edges->push_back({i, i + 1});
    }
    return edges;
}

/// Returns cycle edge-set on [numVertices] vertices.
vector<Edge>* cycleGraphEdge(int numVertices) {
    auto* edges = new vector<Edge>();
    for (int i = 0; i < numVertices; ++i) {
        int j = (i + 1) % numVertices;
        edges->push_back({i, j});
    }
    return edges;
}

/// Returns complete graph edge-set on [numVertices] vertices.
vector<Edge>* completeGraphEdge(int numVertices) {
    auto* edges = new vector<Edge>();
    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            edges->push_back({i, j});
        }
    }
    return edges;
}

/// Returns bipartite graph edge-set on [numVerticesA, numVerticesB] vertices.
vector<Edge>* bipartiteGraphEdge(int numVerticesA, int numVerticesB) {
    auto* edges = new vector<Edge>();
    for (int i = 0; i < numVerticesA; ++i) {
        for (int j = 0; j < numVerticesB; ++j) {  
            edges->push_back({i, numVerticesA + j});
        }
    }
    return edges;
}

/// Returns k-wheel graph edge-set on [numVertices - k, k] vertices.
vector<Edge>* wheelGraphEdge(int numVertices, int k) {
    // make cycle
    auto* edges = cycleGraphEdge(numVertices-k);
    // add edges from partition of k vertices
    for (int j = numVertices - k; j < numVertices; j++) {
        for (int i = 0; i < numVertices - k; ++i) {
            edges->push_back({i, j});
        }
    }
    return edges;
}

//////

////// Graph operations
/// Removes all edges connected to a specific vertex.
void removeVertexEdges(vector<Edge>* edges, int vertex) {
    edges->erase(remove_if(edges->begin(), edges->end(), 
                [vertex](const Edge& e) { return e.first == vertex || e.second == vertex; }),
                edges->end());
}

/// Removes a specific edge.
void removeEdge(vector<Edge>& edges, const Edge& target) {
    edges.erase(
        std::remove(edges.begin(), edges.end(), target),
        edges.end()
    );
}

/// Returns the complement of the given graph
Graph graphComplement(const Graph& g) {
    int n = num_vertices(g);
    Graph G = completeGraph(n);

    for (auto e : make_iterator_range(edges(g))) {
        remove_edge(source(e,g), target(e,g), G);
    }

    return G;
}

/// Returns the union of two graphs over the same vertex set [n-1]={0,...,n-1}
Graph graphUnion(const Graph& g1, const Graph& g2) {
    int n = num_vertices(g1);
    Graph G(n);

    // Copy edges from g1
    for (auto e : make_iterator_range(edges(g1))) {
        int s = source(e, g1);
        int t = target(e, g1);
        add_edge(s, t, G);
    }

    // Copy edges from g2
    for (auto e : make_iterator_range(edges(g2))) {
        int s = source(e, g2);
        int t = target(e, g2);
        add_edge(s, t, G);
    }

    return G;
}

/// Returns the strong product of two graphs.
vector<Edge>* strongProductEdge(const vector<Edge>* graph1, int n1, 
                                const vector<Edge>* graph2, int n2) {
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

/// Returns the 2-blowup of the given graph.
vector<Edge>* blowup(const vector<Edge>* graph, int n) {
    auto* edges = new vector<Edge>();
    for (auto& e : *graph) {
        edges->push_back({e.first, e.second});
        edges->push_back({e.first + n, e.second + n});
        edges->push_back({e.first + n, e.second});
        edges->push_back({e.first, e.second + n});
    }
    return edges;
}
//////

/// Prints progress bar.
void printProgressBar(int progress, int total, string message) {
    float percentage = (float)progress / total;
    float pos = 50.0 * percentage; //50 = width

    cout << "\033[2A\033[2K\r" << message << endl; // Move up and clear line
    cout << "\033[2K\r["; // clear line
    for (int i = 0; i < 50; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(percentage * 100.0) << "%" << endl;
    cout.flush();
}
