//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

set<string> MakeSet(vector<string> query_words);
vector<string> MakeVector(set<string> query_words);
vector<string> SplitIntoWords(const string& text);
set<string> ParseStopWords(const string& text);
vector<string> ParseQuery(string text, set<string> stop_words);

int main() {
    // Read stop words
    string stop_words_joined;
    getline(cin, stop_words_joined);
    set<string> stop_words = ParseStopWords(stop_words_joined);

    // Read query
    string query;
    getline(cin, query);
    vector<string> query_words = ParseQuery(query, stop_words);

    for (string word : query_words) {
        cout << '[' << word << ']' << endl;
    }
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (char c : text) {
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

set<string> ParseStopWords(const string& text) {
    set<string> stop_words;
    for (string& word : SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    return stop_words;
}

vector<string> ParseQuery(string text, set<string> stop_words) {
    vector<string> words;
    for (string word : SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) {
            words.push_back(word);
        }
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