#ifndef SATSOLVER_H
#define SATSOLVER_H

#include "helperFunctions.h"
#include "glucose/core/Solver.h"

using namespace Glucose;

// SAT Solver Wrapper (Using Glucose)
class SatSolver {
public:
    Solver solver;
    vector<Var> vars;
    
    SatSolver(int numEdges) {
        vars.resize(numEdges);
        for (int i = 0; i < numEdges; ++i) {
            vars[i] = solver.newVar();
        }
    }

    // Add a clause (OR constraint)
    void addClause(vector<int> literals) {
        vec<Lit> clause;
        for (int lit : literals) {
            clause.push(mkLit(vars[abs(lit) - 1], lit < 0));
        }
        solver.addClause(clause);
    }

    // Solve SAT problem
    bool solve(vector<int>& assignment) {
        if (!solver.solve()) return false;
        assignment.resize(vars.size());
        for (size_t i = 0; i < vars.size(); ++i) {
            assignment[i] = (solver.modelValue(vars[i]) == l_True);
        }
        return true;
    }
};


#endif // SATSOLVER_H
