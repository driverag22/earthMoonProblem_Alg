#ifndef BIPLANARTESTER_H
#define BIPLANARTESTER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

#include "helperFunctions.h"

/* using namespace boost; */
using namespace std;

/// Global var
extern int maxEdges;

/// Preprocess edge list to order by edge difficulty.
void preprocessEdges(vector<Edge>* edges, int n);

/// Back-tracking recursive implementation of biplanarity checker.
bool isBiplanar(vector<Edge>& edges, int index, Graph& g1, Graph& g2);

/// Determine if given graph is biplanar.
void testBiplanarity(vector<Edge>* edges, int n);

#endif // BIPLANARTESTER_H
