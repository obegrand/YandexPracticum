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

void IsPalindrome(string s) {
	if (s.size() <= 1)
	{
		cout << "1"s << endl;
	}
	else {
		int counter = 0;
		for (int i = 0; i < s.size() / 2; i++)
		{
			if (s[i] == s[s.size() -1 - i])
			{
				++counter;
			}
		}
		if (counter == s.size()/2)
		{
			cout << "1"s << endl;
		}
		else
		{
			cout << "0"s << endl;
		}
	}
}

int Factorial(int x) {
	if (x == 0) { return 1; }
	bool minus = false;
	int rez = 1;
	if (x < 0) { minus = true; x *= -1; }
	for (int i = 1; i < x + 1; i++) {
		rez *= i;
	}
	if (minus && x % 2 == true) { return rez * -1; }
	return rez;
}

int main() {
	for (int i = -10; i < 10; i++)
	{
		cout << Factorial(i) << endl;
	}
	system("pause");
}