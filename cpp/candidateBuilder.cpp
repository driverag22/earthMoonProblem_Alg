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
Graph buildMaximalPlanarGraph(int n, const Graph* avoidGraph) {
    // we start with a simple cycle on n vertices and add edges to it
    Graph G = cycleGraph(n);

    set<Edge> edgesToAvoid;
    if (avoidGraph) {
        for (auto e : make_iterator_range(edges(*avoidGraph))) {
            int u = source(e, *avoidGraph);
            int v = target(e, *avoidGraph);
            if (u > v) std::swap(u, v);
            edgesToAvoid.insert({u, v});
        }
    }

    // We'll attempt all possible edges in random order, adding them if planarity holds.
    vector<pair<int,int>> allPossible;
    allPossible.reserve(n*(n-1)/2);
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            // skip edges already in the cycle, or in the avoidGraph
            if (!edge(i, j, G).second && (!avoidGraph || edgesToAvoid.count({i, j}) == 0))
                allPossible.push_back({i, j});
        }
    }

    // Shuffle edges
    static std::mt19937 rng{ std::random_device{}() };
    shuffle(allPossible.begin(), allPossible.end(), rng);

    // Try adding each edge, keep it if the graph stays planar
    for (auto& e : allPossible) {
        if (canAddEdgePlanar(G, e.first, e.second)) {
            add_edge(e.first, e.second, G);
        }
    }

    return G;
}

/// Function that computes for i=[numVertLow], ...,[numVertHigh] ([numVertLow] \leq [numAttempts])
/// biplanar graphs on i vertices, and determines if they are candidates for high chromatic
/// number (\geq 9 or \geq 10).
void computeCandidateGraphs(int numVertLow, int numVertHigh, int numAttempts, bool ind, bool chr) {
    for (int n = numVertLow; n <= numVertHigh; n++) {
        printProgressBar(0, numAttempts, 
                         "Iteration i = 0/" + to_string(numAttempts) 
                         + ", numb of vertices = " + to_string(n) + "/" + to_string(numVertHigh) + ": ");
        for (int i = 0; i < numAttempts; i++) {
            // build max planar graphs on n vertices
            Graph g1 = buildMaximalPlanarGraph(n);
            // build max planar graphs on n vertices avoiding first graph
            Graph g2 = buildMaximalPlanarGraph(n, &g1);
            cout << num_edges(g1) << " " << num_edges(g2) << endl;
            cout << endl << endl;
            // take their graph union
            Graph g = graphUnion(g1, g2);

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
            printProgressBar(i+1, numAttempts, 
                             "Iteration i = " + to_string(i+1) + "/" + to_string(numAttempts) 
                             + ", numb of vertices = " + to_string(n) + "/" + to_string(numVertHigh) + ": ");
        }
        cout << endl << endl << endl;
    }
}

/// Saves candidate graph [g] and partitions [g1],[g2] at `data/candidates/{txt}{x}/graph_{i}_{n}.txt`
void saveCandidateGraph(Graph g, Graph g1, Graph g2, string txt, int i, int n, int c) {
    string s = txt + to_string(c) + "/" + to_string(n) + "_" + to_string(i) + "_graph";
    outputGraph(g, "candidates/" + s);
    string t = txt + to_string(c) + "/" + to_string(n) + "_" + to_string(i) + "_partitions";
    outputPartitions(g1, g2, "candidates/" + t);
}
