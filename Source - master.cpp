//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

void GetWords() {
	vector<string> words;

	string query;
	getline(cin, query);

	string word;

	for (char c : query) {
		if (c == ' ')
		{
			if (word != "")
			{
				words.push_back(word);
				word = ""s;
			}
		}
		else {
			word += c;
		}
	}
	if (word != "")//добавляем последнее слово
	{
		words.push_back(word);
	}
	//это уже вывод
	/*for (string wordw : words) {
		cout << '[' << wordw << ']' << endl;
	}*/
}

int main() {
	
	system("pause");
}