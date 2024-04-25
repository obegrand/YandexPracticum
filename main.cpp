#include "simple_vector.h"
#include "log_duration.h"

#include <cassert>
#include <iostream>
#include <numeric>

using namespace std;

//#define DEBUG

class X {
public:
	X()
		: X(5) {
	}
	X(size_t num)
		: x_(num) {
	}
	X(const X& other) = delete;
	X& operator=(const X& other) = delete;
	X(X&& other) {
		x_ = exchange(other.x_, 0);
	}
	X& operator=(X&& other) {
		x_ = exchange(other.x_, 0);
		return *this;
	}
	size_t GetX() const {
		return x_;
	}

private:
	size_t x_;
};

SimpleVector<int> GenerateVector(size_t size) {
	SimpleVector<int> v(size);
	iota(v.begin(), v.end(), 1);
	return v;
}

void TestTemporaryObjConstructor() {
	const size_t size = 1000000;
#ifdef DEBUG	
	cout << "Test with temporary object, copy elision" << endl;
#endif DEBUG
	SimpleVector<int> moved_vector(GenerateVector(size));
	assert(moved_vector.GetSize() == size);
#ifdef DEBUG
	cout << "Done!" << endl << endl;
#endif DEBUG
}

void TestTemporaryObjOperator() {
	const size_t size = 1000000;
#ifdef DEBUG
	cout << "Test with temporary object, operator=" << endl;
#endif DEBUG
	SimpleVector<int> moved_vector;
	assert(moved_vector.GetSize() == 0);
	moved_vector = GenerateVector(size);
	assert(moved_vector.GetSize() == size);
#ifdef DEBUG
	cout << "Done!" << endl << endl;
#endif DEBUG
}

void TestNamedMoveConstructor() {
	const size_t size = 1000000;
#ifdef DEBUG
	cout << "Test with named object, move constructor" << endl;
#endif DEBUG
	SimpleVector<int> vector_to_move(GenerateVector(size));
	assert(vector_to_move.GetSize() == size);

	SimpleVector<int> moved_vector(move(vector_to_move));
	assert(moved_vector.GetSize() == size);
	assert(vector_to_move.GetSize() == 0);
#ifdef DEBUG
	cout << "Done!" << endl << endl;
#endif DEBUG
}

void TestNamedMoveOperator() {
	const size_t size = 1000000;
#ifdef DEBUG
	cout << "Test with named object, operator=" << endl;
#endif DEBUG
	SimpleVector<int> vector_to_move(GenerateVector(size));
	assert(vector_to_move.GetSize() == size);

	SimpleVector<int> moved_vector = move(vector_to_move);
	assert(moved_vector.GetSize() == size);
	assert(vector_to_move.GetSize() == 0);
#ifdef DEBUG
	cout << "Done!" << endl << endl;
#endif DEBUG
}

void TestNoncopiableMoveConstructor() {
	const size_t size = 5;
#ifdef DEBUG
	cout << "Test noncopiable object, move constructor" << endl;
#endif DEBUG
	SimpleVector<X> vector_to_move;
	for (size_t i = 0; i < size; ++i) {
		vector_to_move.PushBack(X(i));
	}

	SimpleVector<X> moved_vector = move(vector_to_move);
	assert(moved_vector.GetSize() == size);
	assert(vector_to_move.GetSize() == 0);

	for (size_t i = 0; i < size; ++i) {
		assert(moved_vector[i].GetX() == i);
	}
#ifdef DEBUG
	cout << "Done!" << endl << endl;
#endif DEBUG
}

void TestNoncopiablePushBack() {
	const size_t size = 5;
#ifdef DEBUG
	cout << "Test noncopiable push back" << endl;
#endif DEBUG
	SimpleVector<X> v;
	for (size_t i = 0; i < size; ++i) {
		v.PushBack(X(i));
	}

	assert(v.GetSize() == size);

	for (size_t i = 0; i < size; ++i) {
		assert(v[i].GetX() == i);
	}
#ifdef DEBUG
	cout << "Done!" << endl << endl;
#endif DEBUG
}

void TestNoncopiableInsert() {
	const size_t size = 5;
#ifdef DEBUG
	cout << "Test noncopiable insert" << endl;
#endif DEBUG
	SimpleVector<X> v;
	for (size_t i = 0; i < size; ++i) {
		v.PushBack(X(i));
	}

	// � ������
	v.Insert(v.begin(), X(size + 1));
	assert(v.GetSize() == size + 1);
	assert(v.begin()->GetX() == size + 1);
	// � �����
	v.Insert(v.end(), X(size + 2));
	assert(v.GetSize() == size + 2);
	assert((v.end() - 1)->GetX() == size + 2);
	// � ��������
	v.Insert(v.begin() + 3, X(size + 3));
	assert(v.GetSize() == size + 3);
	assert((v.begin() + 3)->GetX() == size + 3);
#ifdef DEBUG
	cout << "Done!" << endl << endl;
#endif DEBUG
}

void TestNoncopiableErase() {
	const size_t size = 3;
#ifdef DEBUG
	cout << "Test noncopiable erase" << endl;
#endif DEBUG
	SimpleVector<X> v;
	for (size_t i = 0; i < size; ++i) {
		v.PushBack(X(i));
	}

	auto it = v.Erase(v.begin());
	assert(it->GetX() == 1);
#ifdef DEBUG
	cout << "Done!" << endl << endl;
#endif DEBUG
}

int main() {
	long long time = 0;
	size_t counts = 100;
	for (size_t i = 0; i < counts; i++) {
		LogDuration test("test "+to_string(i+1));
		TestTemporaryObjConstructor();
		TestTemporaryObjOperator();
		TestNamedMoveConstructor();
		TestNamedMoveOperator();
		TestNoncopiableMoveConstructor();
		TestNoncopiablePushBack();
		TestNoncopiableInsert();
		TestNoncopiableErase();
		time += test.GetTime();
	}
	cout << "All time is: " << time << endl;
	return 0;
}