#include "helperFunctions.h"
#include "candidateBuilder.h"

int main() {
    cout << endl;
    cout << endl;
    // (numVertLow, numVertHigh, numAttempts, bool independenceNumbe, bool chromaticNumber)
    computeCandidateGraphs(20, 100, 1000, true, false);
    return 0;
}
