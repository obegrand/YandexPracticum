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

bool IsPalindrome(string s) {
	for (int i = 0; i < s.size() / 2; ++i) {
		if (s[i] != s[s.size() - i - 1]) {
			return false;
		}
	}
	return true;
}

vector<string> PalindromeFilter(vector<string> words, int min_length) {
	vector<string> filtered_words;
	for (string word : words)
	{
		if (IsPalindrome(word) && word.size() >= min_length)
		{
			filtered_words.push_back(word);
		}
	}
	return filtered_words;
}

int main() {
	string query;
	getline(cin, query);
	vector<string> words = GetWords(query);
	vector<string> palidrom_words = PalindromeFilter(words, 3);
	for (size_t i = 0; i < palidrom_words.size(); i++)
	{
		cout << palidrom_words[i] << endl;
	}
	system("pause");
}