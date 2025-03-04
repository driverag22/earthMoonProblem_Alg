#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>

#include "helperFunctions.h"

using namespace boost;
using namespace std;

/// Determines if edge (u,v) can be added while maintaining planarity.
bool canAddEdgePlanar(Graph& G, int u, int v);

/// Function that returns a *random* or *constructed* maximal planar graph on n vertices.
Graph buildMaximalPlanarGraph(int n);

#endif // GRAPHBUILDER_H

