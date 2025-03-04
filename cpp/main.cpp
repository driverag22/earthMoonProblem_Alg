#include "candidateBuilder.h"

int main() {
    for (int n = 50; n < 151; n++) {
        cout << n << endl;
        cout << endl;
        for (int i = 0; i < 100; i++) {
            // build max planar graphs on n vertices,
            // take their graph union
            Graph g1 = buildMaximalPlanarGraph(n);
            Graph g2 = buildMaximalPlanarGraph(n);
            Graph g = graphUnion(g1, g2);

            // save graph if chromatic number \geq 9 or 10
            if (chromaticNumberAtLeast(g, 9)) {
                outputGraph(g, "candidates9/graph_" + to_string(i) + "_" + to_string(n));
                outputPartitions(g1, g2, "candidates9/partition_" + to_string(i) + "_" + to_string(n));
                cout << "-------------------------------------------------" << endl;
                cout << "Graph " << i << " is a candidate for 9!" << endl;
                cout << "-------------------------------------------------" << endl;
            }
            if (chromaticNumberAtLeast(g, 10)) {
                outputGraph(g, "candidates10/graph_" + to_string(i) + "_" + to_string(n));
                outputPartitions(g1, g2, "candidates10/partition_" + to_string(i) + "_" + to_string(n));
                cout << "-------------------------------------------------" << endl;
                cout << "Graph " << i << " is a candidate for 10!" << endl;
                cout << "-------------------------------------------------" << endl;
            }
        }
    }
    return 0;
}
