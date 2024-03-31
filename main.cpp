#include <iostream>

using namespace std;

template <typename F>
int FindFloor(int last, F drop, int first = 1) {
	if (last - first <=2) {
		if (drop((last + first) / 2)) {
			return last-1;
		}
		else {
			return last;
		}
	}
	if (drop((last + first) / 2)) {
		FindFloor(last / 2 + first / 2, drop, first);
	}
	else {
		FindFloor(last, drop, last / 2 + first / 2);
	}
}

int main() {
	int t = 0;
	do {
		int n = 8;
		++t;
		int count = 0;
		auto lambda = [t, &count](int f) { ++count; return f >= t; };
		int found = FindFloor(n, lambda);
		cout << "Need to floor "s << t     << endl;
		cout << "Founded floor "s << found << " after "s << count << " drops"s << endl;
		cout << endl;
	} while (t != 8);
	return 0;
}