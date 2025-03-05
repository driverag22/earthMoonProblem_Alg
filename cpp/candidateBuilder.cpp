#include "candidateBuilder.h"
#include <random>

/// Determines if edge (u,v) can be added while maintaining planarity.
bool canAddEdgePlanar(Graph& G, int u, int v) {
    // add edge
    auto e = add_edge(u, v, G).first;
    bool isStillPlanar = isPlanar(G);
    // remove edge
    remove_edge(e, G);
    return isStillPlanar;
}

/// Returns a *random* maximal planar graph on n vertices.
Graph buildMaximalPlanarGraph(int n) {
    // we start with a simple cycle on n vertices and add edges to it
    Graph G = cycleGraph(n);

    // We'll attempt all possible edges in random order, adding them if planarity holds.
    vector<pair<int,int>> allPossible;
    allPossible.reserve(n*(n-1)/2);
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            // skip edges already in the cycle
            allPossible.push_back({i, j});
        }
    }

    // Shuffle edges
    static std::mt19937 rng{ std::random_device{}() };
    shuffle(allPossible.begin(), allPossible.end(), rng);

    // Try adding each edge, keep it if the graph stays planar
    for (auto& e : allPossible) {
        int u = e.first;
        int v = e.second;
        if (edge(u, v, G).second) {
            // already in the graph
            continue;
        }
        if (canAddEdgePlanar(G, u, v)) {
            add_edge(u, v, G);
        }
    }

    return G;
}

/// Function that computes for i=numVertLow, ...,numVertHigh (numVertLow \leq numAttempts)
/// biplanar graphs on i vertices, and checks if they have chromatic number \geq 9 and/or \geq 10.
/// It performs numAttempts attempts per i.
/// If it finds such a graph, the graph and the two partitions are saved at:
///    - `data/candidates{x}/graph_{i}_{n}.txt`
/// where {x} is 9 or 10 (chromatic number), {i} current attempt, and {n} number of vertices of 
/// the graph.
void computeCandidateGraphs(int numVertLow, int numVertHigh, int numAttempts) {
    for (int n = numVertLow; n <= numVertHigh; n++) {
        for (int i = 0; i < numAttempts; i++) {
            printProgressBar(i, numAttempts, 
                             "Iteration i = " + to_string(i) + "/" + to_string(numAttempts) 
                             + ", numb of vertices = " + to_string(n) + "/" + to_string(numVertHigh) + ": ");
            // build max planar graphs on n vertices,
            // take their graph union
            Graph g1 = buildMaximalPlanarGraph(n);
            Graph g2 = buildMaximalPlanarGraph(n);
            Graph g = graphUnion(g1, g2);

            // save graph if chromatic number \geq 8,9 or 10
            if (chromaticNumberAtLeast(g, 8)) {
                string s = "8_" + to_string(i) + "_" + to_string(n);
                outputGraph(g, "candidates/graph_" + s);
                outputPartitions(g1, g2, "candidates/partition_" + s);
                cout << s << endl;
            }
            if (chromaticNumberAtLeast(g, 9)) {
                string s = "9_" + to_string(i) + "_" + to_string(n);
                outputGraph(g, "candidates/graph_" + s);
                outputPartitions(g1, g2, "candidates/partition_" + s);
                cout << s << endl;
            }
            if (chromaticNumberAtLeast(g, 10)) {
                string s = "10_" + to_string(i) + "_" + to_string(n);
                outputGraph(g, "candidates/graph_" + s);
                outputPartitions(g1, g2, "candidates/partition_" + s);
                cout << s << endl;
            }
        }
    }
}
