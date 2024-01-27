//for YandexPracticum from obegrand aka Andrey Melnikov
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int CountAndAddNewDogs(const vector<string>& new_dogs, const map<string, int>& max_amount, map<string, int>& shelter) {
	return count_if(new_dogs.begin(), new_dogs.end(), [&](const string& new_dog) {
		if (shelter[new_dog] + 1 <= max_amount.at(new_dog)) {
			++shelter[new_dog];
			return true;
		} else {
			return false;
		}});
}

int main() {
	map<string, int> shelter{
		{"shepherd"s, 1},
		{"corgi"s, 3},
	};

	map<string, int> max_amount{
		{"shepherd"s, 2},
		{"corgi"s, 3},
		{"shiba inu"s, 1},
	};

	if (CountAndAddNewDogs({ "shepherd"s, "shiba inu"s, "shiba inu"s, "corgi"s }, max_amount, shelter) == 2) {
		return 0;
	}
	else {
		cout << CountAndAddNewDogs({ "shepherd"s, "shiba inu"s, "shiba inu"s, "corgi"s }, max_amount, shelter) << endl;
	}
	system("pause");
}