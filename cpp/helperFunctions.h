#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

using namespace boost;
using namespace std;

/// Graph types
using Graph = adjacency_list<vecS, vecS, undirectedS>;
using Edge = pair<int, int>;

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

/// Checks if the chromatic number of graph g is at least k.
bool chromaticNumberAtLeast(Graph& g, int k);

////// Graph constructors
/// Returns path Graph on [numVertices] vertices.
Graph pathGraph(int numVertices);

/// Returns cycle Graph on [numVertices] vertices.
Graph cycleGraph(int numVertices);

/// Returns complet Graph on [numVertices] vertices.
Graph completeGraph(int numVertices);

/// Returns path edge-set on [numVertices] vertices.
vector<Edge>* pathGraphEdge(int numVertices);

/// Returns cycleGraph edge-set on [numVertices] vertices.
vector<Edge>* cycleGraphEdge(int numVertices);

/// Returns completeGraph edge-set on [numVertices] vertices.
vector<Edge>* completeGraphEdge(int numVertices);
////// 

////// Graph operations
/// Removes all edges connected to a specific vertex.
void removeVertexEdges(vector<Edge>* edges, int vertex);

/// Returns the complement of the given graph
Graph graphComplement(const Graph& g);

/// Returns the union of two graphs over the same vertex set [n-1]={0,...,n-1}
Graph graphUnion(const Graph& g1, const Graph& g2);

/// Returns the strong product of two graphs.
vector<Edge>* strongProduct(const vector<Edge>* graph1, int n1, 
                            const vector<Edge>* graph2, int n2);

/// Prints progress bar.
void printProgressBar(int progress, int total, string message = "");

#endif // HELPERFUNCTIONS_H
