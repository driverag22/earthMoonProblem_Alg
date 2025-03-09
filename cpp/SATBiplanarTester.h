#ifndef SATBIPLANARTESTER_H
#define SATBIPLANARTESTER_H

#include "helperFunctions.h"
#include <boost/graph/adjacency_list.hpp>

/// Determines if the given graph is biplanar using Gurobi's 
/// SAT solver.
bool isBiplanarSAT(const vector<Edge>& edges, int numVertices);

#endif // SATBIPLANARTESTER_H
