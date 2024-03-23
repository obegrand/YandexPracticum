#include "log_duration.h"

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

vector<float> ComputeAvgTemp(const vector<vector<float>>& measures) {
    int dayCount = measures.size();
    int hourCount = 0;
    if (!measures.empty()) { hourCount = measures.at(0).size(); }

    vector<float> result(hourCount,0);
    vector<int> digitsCount(hourCount, 0);
    result.reserve(hourCount);
    digitsCount.reserve(hourCount);

    for (int i = 0; i < dayCount; ++i) {
        for (int j = 0; j < hourCount; j++) {
            
            result[j] += (measures[i][j] > 0. ? measures[i][j] : 0);
            digitsCount[j] += (measures[i][j] > 0. ? 1 : 0);
        }
    }

    for (int i = 0; i < result.size(); i++) {
        if (digitsCount[i] == 0) { result[i] = 0; continue; }
        result[i] /= digitsCount[i];
    }

    return result;
}

vector<float> GetRandomVector(int size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, 100);

    vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

void Test() {
    // 4 дня по 3 измерения
    vector<vector<float>> v = {
        {0, -1, -1},
        {1, -2, -2},
        {2, 3, -3},
        {3, 4, -4}
    };

    // среднее для 0-го измерения (1+2+3) / 3 = 2 (не учитывам 0)
    // среднее для 1-го измерения (3+4) / 2 = 3.5 (не учитывам -1, -2)
    // среднее для 2-го не определено (все температуры отрицательны), поэтому должен быть 0

    assert(ComputeAvgTemp(v) == vector<float>({ 2, 3.5f, 0 }));
}

int main() {
    Test();
    vector<vector<float>> data;
    data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomVector(5000));
    }

    vector<float> avg;
    {
        LOG_DURATION("ComputeAvgTemp"s);
        avg = ComputeAvgTemp(data);
    }

    cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
}