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
    Graph G(n);

    // Add cycle edges
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        add_edge(i, j, G);
    }

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

/// Edge flipping operation to modify the graph in the search process.
/// This is intended to reduce triangles in the complement and potentially increase the chromatic number.
void edgeFlip(Graph& g, Edge e) {

}

/// Simulated annealing based method to search for a graph of thickness two with chromatic number at least [targetChromatic].
/// This function sets-up the initial solution and iteratively refines it.
void simulatedAnnealing(Graph& g, int targetChromatic) {

}

/// Swarm intelligence based method to search for a graph of thickness two with chromatic number at least [targetChromatic].
/// This function sets-up a population of candidate solutions and guides them toward higher chromatic numbers.
void swarmIntelligence(Graph& g, int targetChromatic) {

}
