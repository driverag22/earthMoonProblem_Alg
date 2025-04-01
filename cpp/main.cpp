#include "biplanarTester.h"
#include "candidateBuilder.h"
#include "biplanarSAT.h"

int main() {
    int cols = 3, rows = 3;
    int n = cols * rows;
    vector<Edge> edges = triangularGridMaxPlanarGraphEdge(rows, cols);
    Graph g = constructGraph(edges, n);
    if (!isPlanar(g)) {
	    cout << "not planar!" << endl;
	    return 0;
    }
    edges = blowup(edges, n);
    auto start = chrono::high_resolution_clock::now(); 
    if (isBiplanarSAT(edges, n)) { 
	    cout << "biplanarrr" << endl; 
    } 
    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> elapsed = end - start; 
    cout << "Took: " << elapsed.count() << " seconds.\n"; 
    return 0;
}

