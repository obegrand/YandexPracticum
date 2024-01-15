//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

std::vector<std::string> GetWords(std::string query) {
	std::vector<std::string> words;
	std::string word;
	for (char c : query) {
		if (c == ' ')
		{
			if (word != "") //првоерка на пустоту 
			{
				words.push_back(word);
				word = "";
			}
		}
		else {
			word += c;
		}
	}
	if (word != "") //добавляем последнее слово
	{
		words.push_back(word);
	}
	return words;
}

int main() {
	string query;
	getline(cin, query);
	for (string wordw : GetWords(query)) {
		cout << wordw << " ";
	}
	cout << endl;
	system("pause");
}