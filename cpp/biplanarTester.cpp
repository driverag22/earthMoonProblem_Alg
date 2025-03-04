#include "biplanarTester.h"
int maxEdges;
int c = 0;
std::unordered_map<string, bool> memo;

/// Returns the complement of the given key.
string complementKey(const string& key) {
    string comp = key;
    for (char &c : comp) {
        c = (c == '0' ? '1' : '0');
    }
    return comp;
}

/// Returns the canonical key (the lexicographically smallest 
/// one of key and its complement)
string canonicalKey(const string& key) {
    string comp = complementKey(key);
    return (key < comp ? key : comp);
}

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
    string canonKey = canonicalKey(stateKey);
    if (memo.find(canonKey) != memo.end()) {
        c++;
        cout << c << endl;
        return memo[canonKey];
    }

    if ((size_t)index == edges.size()) {
        bool result = isPlanar(g1) && isPlanar(g2);
        c++;
        cout << c << endl;
        if (result) {
            cout << "Graph is biplanar!" << endl;
            cout << "Number of edges:" << endl;
            cout << "    |E(G1)| = " << num_edges(g1) << endl;
            cout << "    |E(G2)| = " << num_edges(g2) << endl;
            cout << "    |E(G1)| + |E(G2)| = " << num_edges(g1) + num_edges(g2) << endl;
            cout << "    should be = " << edges.size() << endl;
            outputPartitions(g1, g2);
        }
        memo[canonKey] = result;
        return result;
    }
    
    // check edge bounds
    if (num_edges(g1) > (size_t)maxEdges || num_edges(g2) > (size_t)maxEdges) {
        c++;
        cout << c << endl;
        memo[canonKey] = false;
        return false;
    }

    // check still planar
    if (!isPlanar(g1) || !isPlanar(g2)) {
        c++;
        cout << c << endl;
        memo[canonKey] = false;
        return false;
    }

    string newStateKey;
    // go for balanced partitions first
    if (num_edges(g1) <= num_edges(g2)) {
        add_edge(edges[index].first, edges[index].second, g1);
        newStateKey = canonKey + "0";  // means edge index goes to g1
        if (isBiplanar(edges, index + 1, g1, g2, newStateKey)) {
        c++;
        cout << c << endl;
            memo[canonKey] = true;
            return true;
        }
        remove_edge(edges[index].first, edges[index].second, g1);

        newStateKey = canonKey + "1";  // means edge index goes to g2
        add_edge(edges[index].first, edges[index].second, g2);
        if (isBiplanar(edges, index + 1, g1, g2, newStateKey)) {
        c++;
        cout << c << endl;
            memo[canonKey] = true;
            return true;
        }
        remove_edge(edges[index].first, edges[index].second, g2);
    } else {
        add_edge(edges[index].first, edges[index].second, g2);
        newStateKey = canonKey + "1";  // means edge index goes to g2
        if (isBiplanar(edges, index + 1, g1, g2, newStateKey)) {
        c++;
        cout << c << endl;
            memo[canonKey] = true;
            return true;
        }
        remove_edge(edges[index].first, edges[index].second, g2);

        add_edge(edges[index].first, edges[index].second, g1);
        newStateKey = canonKey + "0";  // means edge index goes to g1
        if (isBiplanar(edges, index + 1, g1, g2, newStateKey)) {
        c++;
        cout << c << endl;
            memo[canonKey] = true;
            return true;
        }
        remove_edge(edges[index].first, edges[index].second, g1);
    }
    
    memo[canonKey] = false;
    return false;
}
