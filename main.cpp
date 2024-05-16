#include "libstat.h"

#include <iostream>
using namespace std;

int main() {
    using namespace statistics::tests;
    AggregSum();
    AggregMax();
    AggregMean();
    AggregStandardDeviation();
    AggregMode();
    AggregPrinter();

    cout << "Test passed!"sv << endl;
}