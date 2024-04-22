#include "log_duration.h"

// Tests
#include "simple_vector.h"
#include "tests.h"
#include <cassert>
#include <iostream>

using namespace std;

void TestReserveConstructor() {
    cout << "TestReserveConstructor"s << endl;
    SimpleVector<int> v(Reserve(5));
    assert(v.GetCapacity() == 5);
    assert(v.IsEmpty());
    cout << "Done!"s << endl;
}

void TestReserveMethod() {
    cout << "TestReserveMethod"s << endl;
    SimpleVector<int> v;
    // ������������� 5 ���� � �������
    v.Reserve(5);
    assert(v.GetCapacity() == 5);
    assert(v.IsEmpty());

    // ���������� ��������� capacity �� 1
    v.Reserve(1);
    // capacity ������ �������� �������
    assert(v.GetCapacity() == 5);
    // �������� 10 ��������� � ������
    for (int i = 0; i < 10; ++i) {
        v.PushBack(i);
    }
    assert(v.GetSize() == 10);
    // �������� capacity �� 100
    v.Reserve(100);
    // ��������, ��� ������ �� ���������
    assert(v.GetSize() == 10);
    assert(v.GetCapacity() == 100);
    // ��������, ��� �������� �� �����
    for (int i = 0; i < 10; ++i) {
        assert(v[i] == i);
    }
    cout << "Done!"s << endl;
}

int main() {
    LOG_DURATION("main");
    TestReserveConstructor();
    TestReserveMethod();
}