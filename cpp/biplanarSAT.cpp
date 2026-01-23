#include <future>
#include "biplanarSAT.h"

// Sinz sequential counter encoding for at-most-k on a list of literals.
static void addAtMostK(SatSolver& sat, const vector<Lit>& lits, int k) {
    int m = lits.size();
    if (k <= 0) {
        for (const auto& lit : lits) {
            vec<Lit> clause;
            clause.push(~lit);
            sat.solver.addClause(clause);
        }
        return;
    }
    if (m <= k) return; // no constraint needed

    vector<vector<Lit>> s(m, vector<Lit>(k));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) {
            Var v = sat.solver.newVar();
            s[i][j] = mkLit(v, false);
        }
    }

    // (¬x1 ∨ s1,1)
    {
        vec<Lit> clause;
        clause.push(~lits[0]);
        clause.push(s[0][0]);
        sat.solver.addClause(clause);
    }

    for (int i = 1; i < m; ++i) {
        // (¬xi ∨ si,1)
        {
            vec<Lit> clause;
            clause.push(~lits[i]);
            clause.push(s[i][0]);
            sat.solver.addClause(clause);
        }
        // (¬s(i-1),1 ∨ si,1)
        {
            vec<Lit> clause;
            clause.push(~s[i-1][0]);
            clause.push(s[i][0]);
            sat.solver.addClause(clause);
        }
        for (int j = 1; j < k; ++j) {
            // (¬s(i-1),j-1 ∨ si,j)
            {
                vec<Lit> clause;
                clause.push(~s[i-1][j-1]);
                clause.push(s[i][j]);
                sat.solver.addClause(clause);
            }
            // (¬xi ∨ ¬s(i-1),j ∨ si,j)
            {
                vec<Lit> clause;
                clause.push(~lits[i]);
                clause.push(~s[i-1][j]);
                clause.push(s[i][j]);
                sat.solver.addClause(clause);
            }
        }
    }

    for (int i = k; i < m; ++i) {
        // (¬xi ∨ ¬s(i-1),k)
        vec<Lit> clause;
        clause.push(~lits[i]);
        clause.push(~s[i-1][k-1]);
        sat.solver.addClause(clause);
    }
}

// Biplanar SAT solver.
bool isBiplanarSAT(vector<Edge>& edges, int n) {
    int numEdges = edges.size();
    SatSolver sat(numEdges);
    
    // Cardinality constraints:
    //  at most 3n-6 true variables
    //  at most 3n-6 false variables <=> at least n-(3n-6) true
    int maxPerPartition = 3 * n - 6;
    if (maxPerPartition < 0) maxPerPartition = 0;
    vector<Lit> trueLits, falseLits;
    trueLits.reserve(numEdges);
    falseLits.reserve(numEdges);
    for (int i = 0; i < numEdges; ++i) {
        trueLits.push_back(mkLit(sat.vars[i], false));
        falseLits.push_back(mkLit(sat.vars[i], true)); // counts ¬xi
    }
    addAtMostK(sat, trueLits, maxPerPartition);
    addAtMostK(sat, falseLits, maxPerPartition);

    // SAT Solving Loop
    vector<int> assignment;
    int i = 0;
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
        // run planarity tests async
        auto futurePlanarG0 = std::async(std::launch::async, [&]() {
            return isPlanarSubgraph(g0, &kuratowskiEdges0);
        });
        auto futurePlanarG1 = std::async(std::launch::async, [&]() {
            return isPlanarSubgraph(g1, &kuratowskiEdges1);
        });
        bool planarG0 = futurePlanarG0.get();
        bool planarG1 = futurePlanarG1.get();
        
        if (planarG0 && planarG1) {
            cout << "Graph is biplanar!" << endl;
            /* printGraph(g0); */
            /* printGraph(g1); */
            outputPartitions(g0, g1, "examplePartitions");
            return true;
             // Add a blocking clause to prevent the same solution again
            /* vector<int> blockingClause; */
            /* for (size_t i = 0; i < assignment.size(); ++i) { */
            /*     blockingClause.push_back(assignment[i] ? -(i + 1) : (i + 1)); */
            /* } */
            /* sat.addClause(blockingClause); */
            /* continue; */
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
    
    /* cout << "Graph is NOT biplanar!" << endl; */
    cout << i << endl;
    return false;
}

// Biplanar SAT solver for blow-up, using only matchings of blown up edges.
bool isBiplanarSATBlowUp(vector<Edge>& edges, int n) {
    int numEdges = edges.size();
    SatSolver sat(numEdges);
    
    // SAT Solving Loop
    vector<int> assignment;
    while (sat.solve(assignment)) {
        // Partition Graphs
        Graph g0(2*n), g1(2*n);
        for (size_t i = 0; i < edges.size(); ++i) {
            // if true, g1 gets one matching, g2 gets the other
            if (assignment[i]) {
                add_edge(edges[i].first, edges[i].second, g1);
                add_edge(edges[i].first + n, edges[i].second + n, g1);

                add_edge(edges[i].first + n, edges[i].second, g0);
                add_edge(edges[i].first, edges[i].second + n, g0);
            } else {
                add_edge(edges[i].first, edges[i].second, g0);
                add_edge(edges[i].first + n, edges[i].second + n, g0);

                add_edge(edges[i].first + n, edges[i].second, g1);
                add_edge(edges[i].first, edges[i].second + n, g1);
            }
        }
        
        vector<Edge> kuratowskiEdges0, kuratowskiEdges1;
        // run planarity tests async
        auto futurePlanarG0 = std::async(std::launch::async, [&]() {
            return isPlanarSubgraph(g0, &kuratowskiEdges0);
        });
        auto futurePlanarG1 = std::async(std::launch::async, [&]() {
            return isPlanarSubgraph(g1, &kuratowskiEdges1);
        });
        bool planarG0 = futurePlanarG0.get();
        bool planarG1 = futurePlanarG1.get();
        
        if (planarG0 && planarG1) {
            cout << "Graph blowup is biplanar!" << endl;
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
                // modulo n to get edges in original graph
                int index = (find(edges.begin(), edges.end(), e) - edges.begin() + 1) % n;
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
                // modulo n to get edges in original graph
                int index = (find(edges.begin(), edges.end(), e) - edges.begin() + 1) % n;
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
