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

/// Function that returns a *random* or *constructed* maximal planar graph on [n] vertices.
/// If [avoidGraph] is passed, it will avoid its edges to construct a maximal planar graph 
/// edge-disjoint from it.
Graph buildMaximalPlanarGraph(int n, const Graph* avoidGraph = nullptr);

/// Function that computes for i=numVertLow, ...,numVertHigh (numVertLow \leq numAttempts)
/// biplanar graphs on i vertices, and determines if they are candidates for high chromatic
/// number (\geq 9 or \geq 10).
/// It performs numAttempts attempts per i.
///
/// The way to determine if graphs are candidates depends on [ind] and [chr].
///   [ind] approach checks if independence number is high enough to lowerbound 
///   chromatic number by 9,10
///   [chr] approach checks if chromatic number \geq 9,10 w.h.p.
///
/// If it finds such a graph, the graph and the two partitions are saved at:
///    - `data/candidates/chr{x}/graph_{i}_{n}.txt`
/// where {x} is 9 or 10 (chromatic number), {i} current attempt, and {n} number of vertices of 
/// the graph.
void computeCandidateGraphs(int numVertLow, int numVertHigh, int numAttempts, bool ind, bool chr);

/// Saves candidate graph [g] and partitions [g1],[g2] at `data/candidates/{txt}{x}/graph_{i}_{n}.txt`
void saveCandidateGraph(Graph g, Graph g1, Graph g2, string txt, int i, int n, int c);

#endif // GRAPHBUILDER_H

