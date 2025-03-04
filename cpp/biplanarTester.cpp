#include "biplanarTester.h"
int maxEdges;
std::unordered_map<string, bool> memo;

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
bool isBiplanar(vector<Edge>& edges, int index, Graph& g1, Graph& g2, string stateKey) {
    // Check if state is in memo
    if (memo.find(stateKey) != memo.end()) {
        return memo[stateKey];
    }

    if ((size_t)index == edges.size()) {
        bool result = isPlanar(g1) && isPlanar(g2);
        if (result) {
            cout << "Graph is biplanar!" << endl;
            cout << "Number of edges:" << endl;
            cout << "    |E(G1)| = " << num_edges(g1) << endl;
            cout << "    |E(G2)| = " << num_edges(g2) << endl;
            cout << "    |E(G1)| + |E(G2)| = " << num_edges(g1) + num_edges(g2) << endl;
            cout << "    should be = " << edges.size() << endl;
            outputPartitions(g1, g2);
            return true;
        }
        memo[stateKey] = result;
        return result;
    }
    
    // check edge bounds
    if (num_edges(g1) > (size_t)maxEdges || num_edges(g2) > (size_t)maxEdges) {
        memo[stateKey] = false;
        return false;
    }

    // check still planar
    if (!isPlanar(g1) || !isPlanar(g2)) {
        memo[stateKey] = false;
        return false;
    }

    string newStateKey;
    // go for balanced partitions first
    if (num_edges(g1) <= num_edges(g2)) {
        add_edge(edges[index].first, edges[index].second, g1);
        newStateKey = stateKey + "0";  // means edge index goes to g1
        if (isBiplanar(edges, index + 1, g1, g2, newStateKey)) {
            memo[stateKey] = true;
            return true;
        }
        remove_edge(edges[index].first, edges[index].second, g1);

        newStateKey = stateKey + "1";  // means edge index goes to g2
        add_edge(edges[index].first, edges[index].second, g2);
        if (isBiplanar(edges, index + 1, g1, g2, newStateKey)) {
            memo[stateKey] = true;
            return true;
        }
        remove_edge(edges[index].first, edges[index].second, g2);
    } else {
        add_edge(edges[index].first, edges[index].second, g2);
        newStateKey = stateKey + "1";  // means edge index goes to g2
        if (isBiplanar(edges, index + 1, g1, g2, newStateKey)) {
            memo[stateKey] = true;
            return true;
        }
        remove_edge(edges[index].first, edges[index].second, g2);

        add_edge(edges[index].first, edges[index].second, g1);
        newStateKey = stateKey + "0";  // means edge index goes to g1
        if (isBiplanar(edges, index + 1, g1, g2, newStateKey)) {
            memo[stateKey] = true;
            return true;
        }
        remove_edge(edges[index].first, edges[index].second, g1);
    }
    
    memo[stateKey] = false;
    return false;
}
