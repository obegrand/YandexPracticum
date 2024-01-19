//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

//bool IsPalindrome(string s) {
//	for (int i = 0; i < s.size() / 2; ++i) {
//		if (s[i] != s[s.size() - i - 1]) {
//			return false;
//		}
//	}
//	return true;
//}
//
//vector<string> PalindromeFilter(vector<string> words, int min_length) {
//	vector<string> filtered_words;
//	for (string word : words)
//	{
//		if (IsPalindrome(word) && word.size() >= min_length)
//		{
//			filtered_words.push_back(word);
//		}
//	}
//	return filtered_words;
//}
//
//string query;
//getline(cin, query);
//vector<string> words = GetWords(query);
//vector<string> palidrom_words = PalindromeFilter(words, 3);

int CalculateSimilarity(vector<string> first, vector<string> second) {
	// верните размер пересечения множеств слов first и second
	set<string> similarityWords;
	for (string word1 : first) {
		for (string word2 : second) {
			if (word1 == word2) { similarityWords.insert(word2); }
		}
	}
	return similarityWords.size();
}

vector<string> SplitIntoWords(string text) {
	vector<string> words;
	string word;
	for (const char c : text) {
		if (c == ' ') {
			if (!word.empty()) {
				words.push_back(word);
				word.clear();
			}
		}
		else {
			word += c;
		}
	}
	if (!word.empty()) {
		words.push_back(word);
	}
	return words;
}

int main() {
	string query, description;

	getline(cin, query);
	getline(cin, description);

	cout << CalculateSimilarity(SplitIntoWords(query), SplitIntoWords(description)) << endl;
	system("pause");
}
