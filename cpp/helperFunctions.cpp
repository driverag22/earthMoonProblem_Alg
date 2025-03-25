#include "helperFunctions.h"

/// Returns graph corresponding to given edge list.
Graph constructGraph(vector<Edge>& edges, int numVertices) {
    Graph G(numVertices);
    for (const auto& e : edges)
        add_edge(e.first, e.second, G);
    return G;
}

/// Returns edge list corresponding to given graph.
vector<Edge>* extractEdges(Graph& g) {
    vector<Edge>* edgeSet = new vector<Edge>;
    for (const auto& e : make_iterator_range(edges(g))) {
        edgeSet->emplace_back(source(e,g), target(e,g));
    }
    return edgeSet;
}


/// Returns whether given graph is planar.
bool isPlanar(Graph& g) {
    return boyer_myrvold_planarity_test(g);
}

/// Returns whether given graph is planar, and finds a Kuratowski subgraph 
/// if it is not.
bool isPlanarSubgraph(Graph& g, vector<Edge>* kuratowskiEdges) {
    // If graph is planar, return true
    if (isPlanar(g)) {
        return true;
    }
    assignEdgeIndices(g);

    // If not planar, extract the Kuratowski subgraph
    if (kuratowskiEdges) {
        kuratowskiEdges->clear();
        vector<boost::graph_traits<Graph>::edge_descriptor> kuratowskiEdgesDesc;

        // Call Boost planarity test with edge index map
        // Use Boost's function to get the Kuratowski subgraph
        boyer_myrvold_planarity_test(
            g,
            boost::boyer_myrvold_params::kuratowski_subgraph = back_inserter(kuratowskiEdgesDesc)
        );

        // Convert edge descriptors to pairs of vertex indices
        for (auto e : kuratowskiEdgesDesc) {
            kuratowskiEdges->emplace_back(source(e, g), target(e, g));
        }
    }
    return false;
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

/// Determines if edge (u,v) can be added while maintaining planarity.
bool canAddEdgePlanar(Graph& G, int u, int v) {
    // add edge
    auto e = add_edge(u, v, G).first;
    bool isStillPlanar = isPlanar(G);
    // remove edge
    remove_edge(e, G);
    return isStillPlanar;
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

/// Returns a maximal planar graph edge set with [numVertices] vertices.
vector<Edge> maximalPlanarGraphEdge(int numVertices) {
    vector<Edge> edges = {{0, 1}, {1, 2}, {2, 0}};
    
    for (int i = 3; i < numVertices; i++) {
        edges.emplace_back(i, i - 1);
        edges.emplace_back(i, i - 2);
        edges.emplace_back(i, i - 3);
    }
    return edges;
}

/// Returns a planar graph edge set corresponding to stacking K_4's 
/// on an edge, with [2*numStacks + 2] vertices (each stacked K_4 uses 
/// two new vertices, and the two original edge vertices).
/// Assumes numStacks \geq 1.
vector<Edge> k4stack(int numStacks) {
    vector<Edge> edges = {{0, 1}, {1, 2}, {2, 0}, {3,0}, {3,1}, {3,2}};
    
    for (int i = 4; i < 2*(numStacks + 1); i=i+2) {
        edges.emplace_back(i, 0);
        edges.emplace_back(i, 1);
        edges.emplace_back(i, i+1);
        edges.emplace_back(i+1, 0);
        edges.emplace_back(i+1, 1);
    }
    return edges;
}

/// Returns a path graph edge-set on [numVertices] vertices.
vector<Edge> pathGraphEdge(int numVertices) {
    vector<Edge> edges;
    edges.reserve(numVertices - 1);
    for (int i = 0; i < numVertices - 1; ++i) {
        edges.emplace_back(i, i + 1);
    }
    return edges;
}

/// Returns a cycle graph edge-set on [numVertices] vertices.
vector<Edge> cycleGraphEdge(int numVertices) {
    vector<Edge> edges = pathGraphEdge(numVertices);
    edges.emplace_back(numVertices - 1, 0);
    return edges;
}

/// Returns a complete graph edge-set on [numVertices] vertices.
vector<Edge> completeGraphEdge(int numVertices) {
    vector<Edge> edges;
    edges.reserve((numVertices * (numVertices - 1)) / 2);
    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            edges.emplace_back(i, j);
        }
    }
    return edges;
}

/// Returns a bipartite graph edge-set on [numVerticesA, numVerticesB] vertices.
vector<Edge> bipartiteGraphEdge(int numVerticesA, int numVerticesB) {
    vector<Edge> edges;
    edges.reserve(numVerticesA * numVerticesB);
    for (int i = 0; i < numVerticesA; ++i) {
        for (int j = 0; j < numVerticesB; ++j) {
            edges.emplace_back(i, numVerticesA + j);
        }
    }
    return edges;
}

/// Returns a k-wheel graph edge-set on [numVertices - k, k] vertices.
vector<Edge> wheelGraphEdge(int numVertices, int k) {
    vector<Edge> edges = cycleGraphEdge(numVertices - k);
    edges.reserve(edges.size() + (numVertices - k) * k);
    for (int j = numVertices - k; j < numVertices; j++) {
        for (int i = 0; i < numVertices - k; ++i) {
            edges.emplace_back(i, j);
        }
    }
    return edges;
}

/// Returns an edge-set of a triangular grid maximal planar graph of size [rows x cols].
vector<Edge> triangularGridMaxPlanarGraphEdge(int rows, int cols) {
    vector<Edge> edges;
    auto index = [cols](int r, int c) { return r * cols + c; };
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (c + 1 < cols) edges.emplace_back(index(r, c), index(r, c + 1));
            if (r + 1 < rows) edges.emplace_back(index(r, c), index(r + 1, c));
            if (r + 1 < rows && c + 1 < cols) edges.emplace_back(index(r, c), index(r + 1, c + 1));
        }
    }
    for (int c = 1; c < cols; c++) {
        edges.emplace_back(index(0,0), index(0, c));
        edges.emplace_back(index(0,0), index(rows - 1, c));
    }
    for (int r = 1; r < rows - 1; r++) {
        edges.emplace_back(index(0,0), index(r, 0));
        edges.emplace_back(index(0,0), index(r, cols - 1));
    }
    return edges;
}

/// Returns edge-set of a balanced max planar graph (start with three vertices, 
/// put next vertex in a triangle and connect it to the three vertices).
/// Assumes n \geq 3.
vector<Edge> balancedMaxPlanarGraphEdge(int n) {
    vector<Edge> edges;
    // basic triangle
    edges.push_back({0,1}); edges.push_back({0,2}); edges.push_back({1,2});
    stack<array<int, 3>> Q;
    Q.push({{0,1,2}});
    for (int i = 3; i < n; i++) {
        array<int, 3> vert = Q.top();
        Q.pop();
        for (int v : vert)
            edges.push_back({i,v});
        Q.push({{i, vert[0], vert[1]}});
        Q.push({{i, vert[0], vert[2]}});
        Q.push({{i, vert[1], vert[2]}});
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
vector<Edge> strongProductEdge(const vector<Edge>& graph1, int n1, 
                                const vector<Edge>& graph2, int n2) {
    // create adjacency matrices for G1 and G2 
    // to quickly check if (u,v) is an edge of either
    vector<vector<bool>> adj1(n1, vector<bool>(n1, false));
    vector<vector<bool>> adj2(n2, vector<bool>(n2, false));
    for (const Edge& e : graph1) {
        int u = e.first, v = e.second;
        adj1[u][v] = true;
        adj1[v][u] = true;
    }
    for (const Edge& e : graph2) {
        int u = e.first, v = e.second;
        adj2[u][v] = true;
        adj2[v][u] = true;
    }
    
    vector<Edge> productEdges;
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
                        productEdges.emplace_back(make_pair(index1, index2));
                    }
                }
            }
        }
    }
    
    return productEdges;
}

/// Returns the 2-blowup of the given graph.
vector<Edge> blowup(const vector<Edge>& graph, int n) {
    vector<Edge> edges;
    for (const auto& e : graph) {
        edges.emplace_back(e.first, e.second);
        edges.emplace_back(e.first + n, e.second + n);
        edges.emplace_back(e.first + n, e.second);
        edges.emplace_back(e.first, e.second + n);
    }
    return edges;
}

/// Adds edge indices to [G] (to, e.g., extract Kuratowski subgraph)
void assignEdgeIndices(Graph& G) {
    boost::property_map<Graph, boost::edge_index_t>::type edge_index_map = get(boost::edge_index, G);
    int edge_id = 0;

    // Iterate over all edges in the graph and assign an index
    for (auto e : make_iterator_range(edges(G))) {
        edge_index_map[e] = edge_id++;
    }
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
