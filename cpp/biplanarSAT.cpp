#include "biplanarSAT.h"

// Biplanar SAT solver.
bool isBiplanarSAT(vector<Edge>& edges, int n) {
    int numEdges = edges.size();
    SatSolver sat(numEdges);
    
    // TODO: add cardinality constraints:
    //  at most 3n-6 true variables
    //  at most 3n-6 false variables <=> at least n-(3n-6) true
    
    // SAT Solving Loop
    vector<int> assignment;
    while (sat.solve(assignment)) {
        // Partition Graphs
        Graph g0(n), g1(n);
        for (size_t i = 0; i < edges.size(); ++i) {
            if (assignment[i])
                add_edge(edges[i].first, edges[i].second, g1);
            else
                add_edge(edges[i].first, edges[i].second, g0);
        }
        
        vector<Edge> kuratowskiEdges0, kuratowskiEdges1;
        bool planarG0 = isPlanarSubgraph(g0, &kuratowskiEdges0);
        bool planarG1 = isPlanarSubgraph(g1, &kuratowskiEdges1);
        
        if (planarG0 && planarG1) {
            cout << "Graph is biplanar!" << endl;
            printGraph(g0);
            cout << endl;
            printGraph(g1);
            return true;
        }
        
        // Add constraints to prevent found Kuratowski subgraphs in future iterations
        if (!planarG0) {
            vector<int> clausePositive, clauseNegative;
            // make sure not all Kuratowski edges in the same partition
            for (auto& e : kuratowskiEdges0) {
                int index = find(edges.begin(), edges.end(), e) - edges.begin() + 1;
                clausePositive.push_back(index);  // x_1 OR x_2 OR ... OR x_m
                clauseNegative.push_back(-index); // ¬x_1 OR ¬x_2 OR ... OR ¬x_m
            }
            sat.addClause(clausePositive);
            sat.addClause(clauseNegative);
        }
        if (!planarG1) {
            vector<int> clausePositive, clauseNegative;
            // make sure not all Kuratowski edges in the same partition
            for (auto& e : kuratowskiEdges1) {
                int index = find(edges.begin(), edges.end(), e) - edges.begin() + 1;
                clausePositive.push_back(-index);
                clauseNegative.push_back(index);
            }
            sat.addClause(clausePositive);
            sat.addClause(clauseNegative);
        }
    }
    
    cout << "Graph is NOT biplanar!" << endl;
    return false;
}
