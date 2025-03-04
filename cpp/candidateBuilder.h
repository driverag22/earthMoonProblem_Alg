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

/// Edge flipping operation to modify the graph in the search process.
/// This is intended to reduce triangles in the complement and potentially increase the chromatic number.
void edgeFlip(Graph& g, Edge e);

/// Simulated annealing based method to search for a graph of thickness two with chromatic number at least [targetChromatic].
/// This function sets-up the initial solution and iteratively refines it.
void simulatedAnnealing(Graph& g, int targetChromatic);

/// Swarm intelligence based method to search for a graph of thickness two with chromatic number at least [targetChromatic].
/// This function sets-up a population of candidate solutions and guides them toward higher chromatic numbers.
void swarmIntelligence(Graph& g, int targetChromatic);

#endif // GRAPHBUILDER_H

