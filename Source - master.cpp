//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

vector<string> SplitIntoWords(string text);
set<string> MakeSet(vector<string> query_words);
vector<string> MakeVector(set<string> query_words);
vector<string> RemoveStopWords(set<string> stop_words, vector<string> words);

int main() {
	string query;
	getline(cin, query);
	set<string> stop_words = MakeSet(SplitIntoWords(query));
	getline(cin, query);
	vector<string> words = SplitIntoWords(query);
	vector<string> result = RemoveStopWords(stop_words, words);
	for (string word : result) {
		cout << '[' << word << ']' << endl;
	}
	system("pause");
}

vector<string> RemoveStopWords(set<string> stop_words, vector<string> words) {
	vector<string> result;
	for (int i = 0; i < words.size(); i++){
		if (stop_words.count(words[i])==0){
			result.push_back(words[i]);
		}
	}
	return result;
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

set<string> MakeSet(vector<string> query_words) {
	set<string> result(query_words.begin(), query_words.end());
	return result;
}

vector<string> MakeVector(set<string> query_words) {
	vector<string> result(query_words.begin(), query_words.end());
	return result;
}