#include "helperFunctions.h"
#include "candidateBuilder.h"

int main() {
    cout << endl;
    cout << endl;
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    computeCandidateGraphs(10, 100, 10000, true, true);
    return 0;
}
