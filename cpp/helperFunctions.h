#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

using namespace boost;
using namespace std;

/// Graph types
using Graph = adjacency_list<vecS, vecS, undirectedS>;
using Edge = pair<int, int>;

/// Returns graph corresponding to given edge list.
Graph constructGraph(vector<Edge>& edges, int numVertices);

/// Returns whether given graph is planar.
bool isPlanar(Graph& g);

/// Helper function to output graph.
void outputGraph(Graph& g, string filename = "candidate");

/// Helper function to output edge partitions.
void outputPartitions(Graph& g1, Graph& g2, string filename = "partitions");

/// Prints edges of a given graph (for debugging purposes).
void printGraph(Graph& g);

/// Prints edges of a given edge list (for debugging purposes).
void printEdges(const vector<Edge>& edges);

////// Graph computations
/// Recursively determines if [g] can be colored with [nColors] colors.
/// Returns true if time goes over [time_limit].
bool canColor(const Graph& g, int nColors, vector<int>& colors, int index,
              bool timed, const chrono::steady_clock::time_point& start_time, chrono::milliseconds time_limit);

/// Returns if the chromatic number of graph [g] is at least [k],
/// or if finding a coloring with [k-1] colors takes over [time_limit_s] s 
/// and [timed] is true.
bool chromaticNumberAtLeast(Graph& g, int k, bool time, int time_limit);

/// Determines if there is an independent set in [g] of size [k]
/// containing the vertices in independentSet (backtracking).
bool findIndependentSet(Graph& g, vector<int> independentSet, int index, int k);

/// Checks if the independence number of [g] is at most [k].
bool independenceNumberAtMost(Graph& g, int k);
////// 

////// Graph constructors
/// Returns path Graph on [numVertices] vertices.
Graph pathGraph(int numVertices);

/// Returns cycle Graph on [numVertices] vertices.
Graph cycleGraph(int numVertices);

/// Returns complet Graph on [numVertices] vertices.
Graph completeGraph(int numVertices);

/// Returns a maximal planar graph with [numVertices] vertices.
Graph maximalPlanarGraph(int numVertices);

/// Returns path edge-set on [numVertices] vertices.
vector<Edge>* pathGraphEdge(int numVertices);

/// Returns cycleGraph edge-set on [numVertices] vertices.
vector<Edge>* cycleGraphEdge(int numVertices);

/// Returns complete graph edge-set on [numVertices] vertices.
vector<Edge>* completeGraphEdge(int numVertices);

/// Returns bipartite graph edge-set on [numVerticesA, numVerticesB] vertices.
vector<Edge>* bipartiteGraphEdge(int numVerticesA, int numVerticesB);

/// Returns k-wheel graph edge-set on [numVertices - k, k] vertices.
vector<Edge>* wheelGraphEdge(int numVertices, int k);
////// 

////// Graph operations
/// Removes all edges connected to a specific vertex.
void removeVertexEdges(vector<Edge>* edges, int vertex);

/// Removes a specific edge.
void removeEdge(vector<Edge>& edges, const Edge& target);

/// Returns the complement of the given graph
Graph graphComplement(const Graph& g);

/// Returns the union of two graphs over the same vertex set [n-1]={0,...,n-1}
Graph graphUnion(const Graph& g1, const Graph& g2);

/// Returns the strong product of two graphs.
vector<Edge>* strongProductEdge(const vector<Edge>* graph1, int n1, 
                                const vector<Edge>* graph2, int n2);

/// Returns the 2-blowup of the given graph.
vector<Edge>* blowup(const vector<Edge>* graph, int n);

/// Prints progress bar.
void printProgressBar(int progress, int total, string message = "");

#endif // HELPERFUNCTIONS_H
