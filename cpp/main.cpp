#include <iostream>
#include <limits>
#include "candidateBuilder.h"
#include "biplanarSAT.h"
#include "biplanarTester.h"

using std::cin;
using std::cout;
using std::numeric_limits;
using std::streamsize;

int main() {
    auto askYesNo = [](const char* prompt) {
        cout << prompt << " (y/n): ";
        char response;
        cin >> response;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return response == 'y' || response == 'Y';
    };
    auto readInt = [](const char* prompt, int minValue = 1) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                if (value < minValue) {
                    cout << "Please enter an integer >= " << minValue << ".\n";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cout << "Please enter a valid integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    };

    //////////////////////
    // Candidate finder //
    //////////////////////
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumber, bool chromaticNumber)
    cout << "\nCandidate builder can search for biplanar graphs with chromatic number >= 9 or 10.\n";
    cout << "The two final options control whether to use the independence-number heuristic and\n";
    cout << "whether to run the chromatic-number search (slower, may run up to 1000s per attempt).\n";
    if (askYesNo("Would you like to run computeCandidateGraphs now?")) {
        int numVertLow = readInt("Enter lower number of vertices: ");
        int numVertHigh = readInt("Enter upper number of vertices: ");
        while (numVertHigh < numVertLow) {
            cout << "Upper number of vertices must be at least the lower number.\n";
            numVertHigh = readInt("Enter upper number of vertices: ");
        }
        int numAttempts = readInt("Enter number of attempts per vertex count: ");
        cout << "Independence-number heuristic saves graphs with alpha <= n/10 or n/9\n";
        bool useIndependence = askYesNo("Enable independence-number heuristic?");
        cout << "Chromatic-number search tries to certify chi >= 10 or 9 (up to 1000s per attempt)\n";
        bool useChromatic = askYesNo("Enable chromatic-number search?");
        computeCandidateGraphs(numVertLow, numVertHigh, numAttempts, useIndependence, useChromatic);
    }

    ////////////////////////
    // Blow-up of biwheel //
    ////////////////////////
    // int m = 5;
    // n = m+2; // spokes: 5,6
    // edges = wheelGraphEdge(n, 2);
    // printEdges(edges);
    // cout << endl;
    // edges = blowup(edges, n);
    // printEdges(edges);

    return 0;
}
