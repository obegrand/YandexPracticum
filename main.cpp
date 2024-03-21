#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std;

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
    LogDuration total("Total");
    vector<int> random_bits;
    static const int N = 1 << 17;

    {
        LogDuration append_random("Append random");
        // �������� ������ ���������� ������� 0 � 1
        AppendRandom(random_bits, N);
    }

    vector<int> reversed_bits;
    {
        LogDuration reverse("Reverse");
        // ��������� ������ ����� ������
        reversed_bits = ReverseVector(random_bits);
    }

    {
        LogDuration counting("Counting");
        // ��������� ������� ������ �� ��������� �������� �������
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            double rate = CountPops(reversed_bits, 0, i) * 100. / i;
            cout << "After "s << i << " bits we found "s << rate << "% pops"s << endl;
        }
    }
}

int main() {
    Operate();
}