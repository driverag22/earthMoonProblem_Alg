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

/// Function that computes for i=numVertLow, ...,numVertHigh (numVertLow \leq numAttempts)
/// biplanar graphs on i vertices, and checks if they have chromatic number \geq 9 and/or \geq 10.
/// It performs numAttempts attempts per i.
/// If it finds such a graph, the graph and the two partitions are saved at:
///    - `data/candidates{x}/graph_{i}_{n}.txt`
/// where {x} is 9 or 10 (chromatic number), {i} current attempt, and {n} number of vertices of 
/// the graph.
void computeCandidateGraphs(int numVertLow, int numVertHigh, int numAttempts);

#endif // GRAPHBUILDER_H

