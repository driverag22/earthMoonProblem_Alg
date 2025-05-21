#include "candidateBuilder.h"
#include "biplanarSAT.h"

int main() {
    // int a = 4, b = 4;
    // int n = a*b + 1;
    // vector<Edge> edges = triangularGridMaxPlanarGraphEdge(a,b);
    // vector<Edge> edges = {
    //     // star from 0 (1st spoke)
    //     pair(0, 1),  
    //     pair(0, 2),
    //     pair(0, 3),
    //     pair(0, 4),
    //     pair(0, 5),
    //     // star from 6 (2nd spoke)
    //     pair(1, 6),  
    //     pair(2, 6),
    //     pair(3, 6),
    //     pair(4, 6),
    //     pair(5, 6),
    //     // close cycle
    //     pair(1, 2),
    //     pair(2, 3),
    //     pair(3, 4),
    //     pair(4, 5),
    //     pair(1, 5),
    // };
    // int n = 7;
    int n = 5; // spokes: 4,5
    vector<Edge> edges = wheelGraphEdge(n, 2);
    printEdges(edges);
    cout << endl;
    edges = blowup(edges, n);
    static const array<array<int,3>,6> faces = {{
        {{0,1,3}}, {{0,2,3}}, {{1,2,3}},
        {{0,1,4}}, {{0,2,4}}, {{1,2,4}},
    }};
    int nextVid = 2*n;
    for (auto &tri : faces) {
        int a = tri[0], b = tri[1], c = tri[2];
        // their blown‐up twins are just +n
        int a2 = a + n, b2 = b + n, c2 = c + n;

        // create TWO interior guys per face
        for (int t = 0; t < 2; ++t) {
            int x = nextVid;
            // hook x to both copies of each corner
            edges.emplace_back(a,  x);
            edges.emplace_back(a2, x);
            edges.emplace_back(b,  x);
            edges.emplace_back(b2, x);
            edges.emplace_back(c,  x);
            edges.emplace_back(c2, x);
            nextVid++;
        }
    }
    printEdges(edges);

    /* auto start = chrono::high_resolution_clock::now(); */
    /* if (isBiplanarSAT(edges, n)) { */
    /*     cout << "biplanarrr" << endl; */
    /* } */
    /* auto end = chrono::high_resolution_clock::now(); */
    /* chrono::duration<double> elapsed = end - start; */
    /* cout << "Took: " << elapsed.count() << " seconds.\n"; */
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    /* computeCandidateGraphs(20, 80, 100000, true, true); */
    return 0;
}

