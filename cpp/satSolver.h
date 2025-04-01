#ifndef SATSOLVER_H
#define SATSOLVER_H

#include "helperFunctions.h"
#include "minisat/core/Solver.h"

// SAT Solver Wrapper (Using MiniSat)
class SatSolver {
public:
    Minisat::Solver solver;
    vector<Minisat::Var> vars;
    
    SatSolver(int numEdges) {
        vars.resize(numEdges);
        for (int i = 0; i < numEdges; ++i) {
            vars[i] = solver.newVar();
        }
    }

    // Add a clause (OR constraint)
    void addClause(vector<int> literals) {
        Minisat::vec<Minisat::Lit> clause;
        for (int lit : literals) {
            clause.push(Minisat::mkLit(vars[abs(lit) - 1], lit < 0));
        }
        solver.addClause(clause);
    }

    // Solve SAT problem
    bool solve(vector<int>& assignment) {
        if (!solver.solve()) return false;
        assignment.resize(vars.size());
        for (size_t i = 0; i < vars.size(); ++i) {
            assignment[i] = (solver.modelValue(vars[i]) == Minisat::l_True);
        }
        return true;
    }
};


#endif // SATSOLVER_H
