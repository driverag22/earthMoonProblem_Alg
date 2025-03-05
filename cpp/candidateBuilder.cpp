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

/// Function that computes for i=[numVertLow], ...,[numVertHigh] ([numVertLow] \leq [numAttempts])
/// biplanar graphs on i vertices, and determines if they are candidates for high chromatic
/// number (\geq 9 or \geq 10).
/// It performs [numAttempts] attempts per i.
///
/// The way to determine if graphs are candidates depends on [ind] and [chr].
///   [ind] approach checks if independence number is high enough to lowerbound 
///   chromatic number by 9,10
///   [chr] approach checks if chromatic number \geq 9,10 w.h.p.
///
/// If it finds such a graph, the graph and the two partitions are saved at:
///    - `data/candidates/chr{x}/graph_{i}_{n}.txt`
/// where {x} is 9 or 10 (chromatic number), {i} current attempt, and {n} number of vertices of 
/// the graph.
void computeCandidateGraphs(int numVertLow, int numVertHigh, int numAttempts, bool ind, bool chr) {
    for (int n = numVertLow; n <= numVertHigh; n++) {
        for (int i = 0; i < numAttempts; i++) {
            printProgressBar(i+1, numAttempts, 
                             "Iteration i = " + to_string(i+1) + "/" + to_string(numAttempts) 
                             + ", numb of vertices = " + to_string(n) + "/" + to_string(numVertHigh) + ": ");
            // build max planar graphs on n vertices,
            // take their graph union
            Graph g1 = buildMaximalPlanarGraph(n);
            Graph g2 = buildMaximalPlanarGraph(n);
            Graph g = graphUnion(g1, g2);
            int n = num_vertices(g);

            // save graph if chromatic number 
            // (possbly) \geq 10 or 9
            // we use: 
            //     \chi \geq n/(\alpha) \geq 10,9 
            //                <=> 
            //          \alpha \leq n/10, n/9
            //    OR
            //     \chi \geq 10,9 or over 1000 seconds 
            //     pass while coloring with 9,8 colors.
            if (ind) {
                if (independenceNumberAtMost(g, n/10)) {
                    saveCandidateGraph(g, g1, g2, "ind", i, n, 10);
                } else if (independenceNumberAtMost(g, n/9)) {
                    saveCandidateGraph(g, g1, g2, "ind", i, n, 9);
                }
            }
            if (chr) {
                // wait 1000s ~ 16.6 minutes max.
                if (chromaticNumberAtLeast(g, 10, true, 1000)) {
                    saveCandidateGraph(g, g1, g2, "chr", i, n, 10);
                } else if (chromaticNumberAtLeast(g, 9, true, 1000)) {
                    saveCandidateGraph(g, g1, g2, "chr", i, n, 9);
                }
            }
        }
        cout << endl << endl << endl;
    }
}

/// Saves candidate graph [g] and partitions [g1],[g2] at `data/candidates/{txt}{x}/graph_{i}_{n}.txt`
void saveCandidateGraph(Graph g, Graph g1, Graph g2, string txt, int i, int n, int c) {
    string s = txt + to_string(c) + "/graph_" + to_string(i) + "_" + to_string(n);
    outputGraph(g, "candidates/" + s);
    outputPartitions(g1, g2, "candidates/" + s);
}
