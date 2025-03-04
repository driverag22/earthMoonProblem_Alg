#include "graphBuilder.h"

/// Checks if the chromatic number of graph g is at least k.
bool hasChromaticNumber(Graph& g, int k);

/// Helper function to output the two edge partitions (the two planar subgraphs).
void outputPartitions(Graph& g1, Graph& g2);

/// Prints the edges of a graph (useful for debugging).
void printEdges(const vector<Edge>& edges);

////// Graph constructors (reuse or extend as needed)
/// Returns a path graph edge-set on [numVertices] vertices.
vector<Edge>* pathGraph(int numVertices);

/// Returns a cycle graph edge-set on [numVertices] vertices.
vector<Edge>* cycleGraph(int numVertices);

/// Returns a complete graph edge-set on [numVertices] vertices.
vector<Edge>* completeGraph(int numVertices);

////// Graph operations
/// Removes all edges incident to a specific vertex.
void removeVertexEdges(vector<Edge>* edges, int vertex);

/// Returns the strong product of two graphs.
vector<Edge>* strongProduct(const vector<Edge>* graph1, int n1, 
                              const vector<Edge>* graph2, int n2);

/// Preprocesses an edge list to order edges by “difficulty.”
void preprocessEdges(vector<Edge>* edges, int n);

////// Algorithmic Frameworks for the Chromatic Problem

/// Edge flipping operation to modify the graph in the search process.
/// This is intended to reduce triangles in the complement and potentially increase the chromatic number.
void edgeFlip(Graph& g, Edge e);

/// Simulated annealing based method to search for a graph of thickness two with chromatic number at least [targetChromatic].
/// This function sets-up the initial solution and iteratively refines it.
void simulatedAnnealing(Graph& g, int targetChromatic);

/// Swarm intelligence based method to search for a graph of thickness two with chromatic number at least [targetChromatic].
/// This function sets-up a population of candidate solutions and guides them toward higher chromatic numbers.
void swarmIntelligence(Graph& g, int targetChromatic);
