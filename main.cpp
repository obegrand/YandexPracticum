#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
using namespace chrono;

vector<int> ReverseVector(const vector<int>& source_vector) {
    vector<int> res;
    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }
    return res;
}

int CountPops(const vector<int>& source_vector, int begin, int end) {
    int res = 0;
    for (int i = begin; i < end; ++i) {
        if (source_vector[i]) {
            ++res;
        }
    }
    return res;
}

void AppendRandom(vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) {
        v.push_back(rand() % 2);
    }
}

void Operate() {
    vector<int> random_bits;
    static const int N = 1 << 17;

    {
        const auto start_time = steady_clock::now();
        AppendRandom(random_bits, N);
        const auto end_time = steady_clock::now();
        const auto dur = end_time - start_time;
        cerr << "Append random: "s << chrono::duration_cast<chrono::milliseconds>(dur).count() << " ms"s << endl;
    }
    const auto start_time = steady_clock::now();
    vector<int> reversed_bits = ReverseVector(random_bits);
    const auto end_time = steady_clock::now();
    const auto dur = end_time - start_time;
    cerr << "Reverse: "s << chrono::duration_cast<chrono::milliseconds>(dur).count() << " ms"s << endl;

    {
        const auto start_time = steady_clock::now();
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            double rate = CountPops(reversed_bits, 0, i) * 100. / i;
            cout << "After "s << i << " bits we found "s << rate << "% pops"s << endl;
        }
        const auto end_time = steady_clock::now();
        const auto dur = end_time - start_time;
        cerr << "Counting: "s << chrono::duration_cast<chrono::milliseconds>(dur).count() << " ms"s << endl;
    }
}

int main() {
    Operate();
    return 0;
}
