#include <iostream>
#include <vector>
#include <fstream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

using namespace boost;
using namespace std;

// graph types
using Graph = adjacency_list<vecS, vecS, undirectedS>;
using Edge = std::pair<int, int>;

int n = 8;
int maxEdges = 3 * n - 6;

// strategy design pattern
bool isPlanar(Graph& g) {
    return boyer_myrvold_planarity_test(g);
}

// helper function to output edge partitions
void outputPartitions(const vector<Edge>& partition1, const vector<Edge>& partition2) {
    ofstream file("partitions.txt");
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


// recursion
bool recursion(vector<Edge>& edges, int index, Graph& g1, Graph& g2, vector<Edge>& partition1, vector<Edge>& partition2) {
    // base case
    if (index == edges.size()) {
        if (isPlanar(g1) && isPlanar(g2)) {
            cout << "Graph is biplanar!" << endl;
            outputPartitions(partition1, partition2);
            return true;
        } else {
            return false;
        }
    }
    
    // backtracking
    if (num_edges(g1) > maxEdges || num_edges(g2) > maxEdges) {
        return false;
    }
    
    // adding edge to the first partition
    add_edge(edges[index].first, edges[index].second, g1);
    partition1.push_back(edges[index]);
    if (recursion(edges, index + 1, g1, g2, partition1, partition2)) return true;
    partition1.pop_back();
    remove_edge(edges[index].first, edges[index].second, g1);
    
    // adding edge to the second partition
    add_edge(edges[index].first, edges[index].second, g2);
    partition2.push_back(edges[index]);
    if (recursion(edges, index + 1, g1, g2, partition1, partition2)) return true;
    partition2.pop_back();
    remove_edge(edges[index].first, edges[index].second, g1);
    
    return false;
}

int main() {
    vector<Edge> edges = { {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4} };
    Graph g1(n), g2(n);
    vector<Edge> partition1, partition2;

    if(!recursion(edges, 0, g1, g2, partition1, partition2)) cout << "not biplanar" << endl;
    
    return 0;
}
