#ifndef BIPLANARSAT_H
#define BIPLANARSAT_H

#include "helperFunctions.h"
#include "satSolver.h"

// Biplanar SAT solver.
bool isBiplanarSAT(vector<Edge>& edges, int n);

// Biplanar SAT solver for blow-up, using only matchings of blown up edges.
bool isBiplanarSATBlowUp(vector<Edge>& edges, int n);

#endif // BIPLANARSAT_H
