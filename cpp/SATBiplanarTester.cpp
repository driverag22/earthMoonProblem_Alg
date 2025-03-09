#include "SATBiplanarTester.h"
#include "gurobi_c++.h"

// The custom Gurobi callback class
class PlanarityCallback : public GRBCallback {
    const vector<Edge>& edges;
    int numVertices;
    vector<GRBVar>& x;

public:
    PlanarityCallback(const vector<Edge>& _edges, int _numVertices, vector<GRBVar>& _x)
        : edges(_edges), numVertices(_numVertices), x(_x) {}

protected:
    void callback() override {
        try {
            if (where == GRB_CB_MIPSOL) {
                // Retrieve current solution for each edge variable.
                vector<double> sol(x.size());
                for (size_t i = 0; i < x.size(); i++) {
                    sol[i] = getSolution(x[i]);
                }
                // Build two graphs, one for each partition.
                Graph g0(numVertices);  // Partition where x[i] == 0
                Graph g1(numVertices);  // Partition where x[i] == 1

                // Add edges to the graphs according to the current solution.
                for (size_t i = 0; i < edges.size(); i++) {
                    int u = edges[i].first;
                    int v = edges[i].second;
                    if (sol[i] > 0.5)
                        add_edge(u, v, g1);
                    else
                        add_edge(u, v, g0);
                }

                // Test planarity for both graphs and forbid them if not planar
                if (!isPlanar(g0)) {
                    GRBLinExpr expr = 0;
                    for (size_t i = 0; i < edges.size(); i++) {
                        // add x[i] if corresponds to g(0)
                        if (sol[i] <= 0.5) {
                            expr += x[i];
                        }
                    }
                    // make sure not all edges on the same partition again
                    addLazy(expr >= 1);
                    addLazy(expr <= num_edges(g0) - 1);
                }
                if (!isPlanar(g1)) {
                    GRBLinExpr expr = 0;
                    for (size_t i = 0; i < edges.size(); i++) {
                        // add x[i] if corresponds to g(1)
                        if (sol[i] <= 0.5) {
                            expr += x[i];
                        }
                    }
                    // make sure not all edges on the same partition again
                    addLazy(expr >= 1);
                    addLazy(expr <= num_edges(g1) - 1);
                }
            }
        } catch (GRBException &e) {
            cerr << "Error in callback: " << e.getMessage() << endl;
        }
    }
};

bool isBiplanarSAT(const vector<Edge>& edges, int numVertices) {
    try {
        // Create Gurobi Environment and Model
        GRBEnv env;
        GRBModel model(env);

        int numEdges = edges.size();
        vector<GRBVar> x(numEdges);

        // Decision variables:
        //  - x[i] == 0: edge i in partition 0
        //  - x[i] == 1: edge i in partition 1
        for (int i = 0; i < numEdges; i++) {
            x[i] = model.addVar(0, 1, 0, GRB_BINARY);
        }

        // fix first edge to first partition to avoid symmetric cases
        model.addConstr(x[0] == 0);
        // each partition can have at most 3n - 6 edges
        GRBLinExpr sum_x = 0;
        for (int i = 0; i < numEdges; i++) {
            sum_x += x[i];
        }
        model.addConstr(sum_x <= 3 * numVertices - 6);
        model.addConstr(numEdges - sum_x <= 3 * numVertices - 6); // sum (1 - x_i) ≤ 3n - 6


        // Set up lazy constraints callback
        PlanarityCallback cb(edges, numVertices, x);
        model.setCallback(&cb);
        model.set(GRB_IntParam_LazyConstraints, 1); // Enable lazy constraints
        model.set(GRB_IntParam_OutputFlag, 0); // Disable standard Gurobi output

        // Optimize the model
        model.optimize();

        // Check if a solution was found
        if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
            // Extract solutions
            Graph g1 = Graph(numVertices);
            Graph g2 = Graph(numVertices);
            for (int k = 0; k < numEdges; k++) {
                if (x[k].get(GRB_DoubleAttr_X) > 0.5) {
                    add_edge(edges[k].first, edges[k].second, g1);
                } else {
                    add_edge(edges[k].first, edges[k].second, g2);
                }
            }
            if (!isPlanar(g1)) {
                cout << "g1 not planar" << endl;
                return false;
            }
            if (!isPlanar(g2)) {
                cout << "g2 not planar" << endl;
                return false;
            }
            outputPartitions(g1, g2);
            return true;
        } else if (model.get(GRB_IntAttr_Status) == GRB_INFEASIBLE) {
            cerr << "Model is infeasible. No biplanar partitioning exists." << endl;
            return false;
        } else {
            return false;
        }
    } catch (GRBException& e) {
        cerr << "Gurobi Error: " << e.getMessage() << endl;
        return false;
    }
}

