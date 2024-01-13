//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

void Hause_numbers() {
	vector<string> color = { "","", "", "" };
	vector<int> floor = { 0,0,0,0 };
	for (size_t i = 0; i < 4; i++)
	{
		cin >> color[i];
		cin >> floor[i];
	}
	for (size_t i = 0; i < 3; i++)
	{
		if (color[i] == color[3]&& floor[i] == floor[3]){
			cout << i + 1 << endl;
		}
		if ((color[3] == "?" || floor[3] == -1) && (color[i] == color[3] || floor[i] == floor[3])) {
			cout << i + 1 << endl;
		}
		if (color[3] == "?" && floor[3] == -1) {
			cout << i + 1 << endl;
		}
	}
}

int main() {
	srand(time(NULL));
	int x, y;
	cin >> x >> y;
	if (x < y) {
		for (int i = x; i < y + 1; ++i) {
			cout << i << endl;
		}
	}
	if (x > y) {
		for (int i = y; i > x; --i) {
			cout << i << endl;
		}
	}for (int i = x; i < y + 1; ++i) {
		cout << i << endl;
	}
	system("pause");
}