#include "candidateBuilder.h"
#include "biplanarSAT.h"
#include "biplanarTester.h"

#include <fstream>
#include <chrono>
#include <string>
#include <limits>

using namespace std;

static bool loadGraphFromFile(const string& path, vector<Edge>& edges, int& n) {
    ifstream in(path);
    if (!in.is_open()) {
        cerr << "Could not open file: " << path << ". Please check that it exists and is readable." << endl;
        return false;
    }
    int u, v;
    n = 0;
    while (in >> u >> v) {
        edges.emplace_back(u, v);
        n = max(n, max(u, v) + 1);
    }
    if (edges.empty()) {
        cerr << "File contained no edges or invalid format." << endl;
        return false;
    }
    in.close();
    return true;
}

int main() {
    cout << "Would you like to run biplanar tester (y/n)? ";
    char choice;
    if (!(cin >> choice)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 'y' || choice == 'Y') {
        cout << "Path to file: ";
        string path;
        getline(cin, path);

        int n; vector<Edge> edges;
        if (!path.empty()) {
            if (!loadGraphFromFile(path, edges, n)) return 1;
        } else {
            cout << "No path provided. Running default test graph (5x4 strong product minus vertex 0)." << endl;
            int a = 5, b = 4;
            n = a * b;
            edges = strongProductEdge(pathGraphEdge(a), a, completeGraphEdge(b), b);
            removeVertexEdges(edges, 0);
        }

        auto start = chrono::high_resolution_clock::now();
        testBiplanarity(&edges, n);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "Took: " << elapsed.count() << " seconds.\n";
        return 0;
    }

    cout << "Would you like to run the candidate builder (y/n)? ";
    char build;
    if (!(cin >> build)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (build != 'y' && build != 'Y') return 0;

    int low, high, attempts;
    cout << "Enter lower number of vertices: ";
    if (!(cin >> low)) return 0;
    cout << "Enter higher number of vertices: ";
    if (!(cin >> high)) return 0;
    cout << "Enter number of candidates to build (attempts) per vertex count: ";
    if (!(cin >> attempts)) return 0;
    if (low > high || attempts <= 0 || low <= 0 || high <= 0) {
        cerr << "Invalid numeric input. Ensure 0 < lower <= higher and attempts > 0." << endl;
        return 1;
    }

    cout << "Enable independence-number heuristic? (y/n): ";
    char indChoice; if (!(cin >> indChoice)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enable chromatic-number search? (y/n): ";
    char chrChoice; if (!(cin >> chrChoice)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool ind = (indChoice == 'y' || indChoice == 'Y');
    bool chr = (chrChoice == 'y' || chrChoice == 'Y');

    computeCandidateGraphs(low, high, attempts, ind, chr);
    return 0;
}
