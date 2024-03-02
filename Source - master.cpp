//for YandexPracticum from obegrand aka Andrey Melnikov
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <optional>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;
const float EPSILON = 1e-6;

string ReadLine() {
	string s;
	getline(cin, s);
	return s;
}

int ReadLineWithNumber() {
	int result;
	cin >> result;
	ReadLine();
	return result;
}

vector<string> SplitIntoWords(const string& text) {
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

struct Document {
	Document() = default;

	Document(int id, double relevance, int rating)
		: id(id)
		, relevance(relevance)
		, rating(rating) {
	}

	int id = 0;
	double relevance = 0.0;
	int rating = 0;
};

enum class DocumentStatus {
	ACTUAL,
	IRRELEVANT,
	BANNED,
	REMOVED,
};

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
	set<string> non_empty_strings;
	for (const string& str : strings) {
		if (!str.empty()) {
			non_empty_strings.insert(str);
		}
	}
	return non_empty_strings;
}

class SearchServer {
public:
	template <typename StringContainer>
	explicit SearchServer(const StringContainer& stop_words) : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
		if (!IsValidStopWord()) { throw invalid_argument("The document contains special characters"); };
	}

	explicit SearchServer(const string& stop_words_text) : SearchServer(SplitIntoWords(stop_words_text)) {
		if (!IsValidStopWord()) { throw invalid_argument("The document contains special characters"); };
	}

	void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
		if (documents_.count(document_id) > 0) { throw invalid_argument("ID already exists"); }
		if (document_id < 0) { throw invalid_argument("ID less than 0"); }
		if (!IsValidWord(document)) { throw invalid_argument("The document contains special characters"); }
		const vector<string> words = SplitIntoWordsNoStop(document);
		const double inv_word_count = 1.0 / words.size();
		for (const string& word : words) {
			word_to_document_freqs_[word][document_id] += inv_word_count;
		}
		documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });
		documents_index_.push_back(document_id);
	}

	template <typename DocumentPredicate>
	vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
		const Query query = ParseQuery(raw_query);
		auto matched_documents = FindAllDocuments(query, document_predicate);

		sort(matched_documents.begin(), matched_documents.end(),
			[](const Document& lhs, const Document& rhs) {
				if (abs(lhs.relevance - rhs.relevance) < EPSILON) {
					return lhs.rating > rhs.rating;
				}
				else {
					return lhs.relevance > rhs.relevance;
				}
			});
		if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
			matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		}
		return matched_documents;
	}

	vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
		return FindTopDocuments(
			raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
				return document_status == status;
			});
	}

	vector<Document> FindTopDocuments(const string& raw_query) const {
		return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
	}

	tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
		const Query query = ParseQuery(raw_query);
		auto status = documents_.at(document_id).status;
		vector<string> matched_words;
		for (const string& word : query.plus_words) {
			if (word_to_document_freqs_.count(word) == 0) {
				continue;
			}
			if (word_to_document_freqs_.at(word).count(document_id)) {
				matched_words.push_back(word);
			}
		}
		for (const string& word : query.minus_words) {
			if (word_to_document_freqs_.count(word) == 0) {
				continue;
			}
			if (word_to_document_freqs_.at(word).count(document_id)) {
				matched_words.clear();
				break;
			}
		}
		return { matched_words, status };
	}

	int GetDocumentCount() const {
		return documents_.size();
	}

	int GetDocumentId(int index) const {
		return documents_index_.at(index);
	}

private:
	struct DocumentData {
		int rating;
		DocumentStatus status;
	};
	const set<string> stop_words_;
	map<string, map<int, double>> word_to_document_freqs_;
	map<int, DocumentData> documents_;
	vector<int> documents_index_;

	bool IsValidStopWord() const {
		for (const string& word : stop_words_) {
			if (!IsValidWord(word)) { return false; }
		}
		return true;
	}

	static bool IsValidWord(const string& word) {
		for (const auto symbol : word) {
			if (symbol >= 0 && symbol <= 31) { return false; }
		}
		return true;
	}

	static bool IsValidMinusWord(const string& word) {
		if (word.empty()) {
			return false;
		}
		if (word == "-"s) {
			return false;
		}
		if (word[0] == '-' && word[1] == '-') {
			return false;
		}

		return true;
	}

	bool IsStopWord(const string& word) const {
		return stop_words_.count(word) > 0;
	}

	vector<string> SplitIntoWordsNoStop(const string& text) const {
		vector<string> words;
		for (const string& word : SplitIntoWords(text)) {
			if (!IsStopWord(word)) {
				words.push_back(word);
			}
		}
		return words;
	}

	static int ComputeAverageRating(const vector<int>& ratings) {
		if (ratings.empty()) {
			return 0;
		}
		int rating_sum = 0;
		for (const int rating : ratings) {
			rating_sum += rating;
		}
		return rating_sum / static_cast<int>(ratings.size());
	}

	struct QueryWord {
		string data;
		bool is_minus;
		bool is_stop;
	};

	QueryWord ParseQueryWord(string text) const {
		bool is_minus = false;
		if (text.empty()) { throw invalid_argument("Query is empty"); }
		if (!IsValidWord(text)) { throw invalid_argument("Query contains special characters"); }
		if (!IsValidMinusWord(text)) { throw invalid_argument("Error in query"); }
		if (text[0] == '-') {
			is_minus = true;
			text = text.substr(1);
		}
		return { text, is_minus, IsStopWord(text) };
	}

	struct Query {
		set<string> plus_words;
		set<string> minus_words;
	};

	Query ParseQuery(const string& text) const {
		Query query;
		for (const string& word : SplitIntoWords(text)) {
			const QueryWord query_word = ParseQueryWord(word);
			if (!query_word.is_stop) {
				if (query_word.is_minus) {
					query.minus_words.insert(query_word.data);
				}
				else {
					query.plus_words.insert(query_word.data);
				}
			}
		}
		return query;
	}

	// Existence required
	double ComputeWordInverseDocumentFreq(const string& word) const {
		return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
	}

	template <typename DocumentPredicate>
	vector<Document> FindAllDocuments(const Query& query,
		DocumentPredicate document_predicate) const {
		map<int, double> document_to_relevance;
		for (const string& word : query.plus_words) {
			if (word_to_document_freqs_.count(word) == 0) {
				continue;
			}
			const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
			for (const auto& [document_id, term_freq] : word_to_document_freqs_.at(word)) {
				const auto& document_data = documents_.at(document_id);
				if (document_predicate(document_id, document_data.status, document_data.rating)) {
					document_to_relevance[document_id] += term_freq * inverse_document_freq;
				}
			}
		}

		for (const string& word : query.minus_words) {
			if (word_to_document_freqs_.count(word) == 0) {
				continue;
			}
			for (const auto& [document_id, _] : word_to_document_freqs_.at(word)) {
				document_to_relevance.erase(document_id);
			}
		}

		vector<Document> matched_documents;
		for (const auto& [document_id, relevance] : document_to_relevance) {
			matched_documents.push_back(
				{ document_id, relevance, documents_.at(document_id).rating });
		}
		return matched_documents;
	}
};

void PrintDocument(const Document& document) {
	cout << "{ "s
		<< "document_id = "s << document.id << ", "s
		<< "relevance = "s << document.relevance << ", "s
		<< "rating = "s << document.rating << " }"s << endl;
}

int main() {
	setlocale(LC_ALL, "ru");
	// ѕроверка стоп-слов на спец-символы
	try {
		SearchServer search_server("и в \x12на"s);
	}
	catch (const invalid_argument& error) {
		cout << "Error: " << error.what() << endl;
	}

	// ѕроверка id меньше 0
	try {
		SearchServer search_server("и в на"s);
		search_server.AddDocument(-1, "пушистый кот и модный ошейник"s, DocumentStatus::ACTUAL, { 1, 2 });
	}
	catch (const invalid_argument& error) {
		cout << "Error: " << error.what() << endl;
	}

	// ѕроверка на повтор€ющиес€ id
	try {
		SearchServer search_server("и в на"s);
		search_server.AddDocument(0, "пушистый кот и модный ошейник"s, DocumentStatus::ACTUAL, { 1, 2 });
		search_server.AddDocument(0, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
	}
	catch (const invalid_argument& error) {
		cout << "Error: " << error.what() << endl;
	}

	// ѕроверка документа на спец-символы
	try {
		SearchServer search_server("и в на"s);
		search_server.AddDocument(0, "пушистый кот и модн\x12ый ошейник"s, DocumentStatus::ACTUAL, { 1, 2 });
	}
	catch (const invalid_argument& error) {
		cout << "Error: " << error.what() << endl;
	}

	// ѕроверка на спец символе в поисковом запросе
	try {
		SearchServer search_server("и в на"s);
		search_server.AddDocument(0, "пушистый кот и модный ошейник"s, DocumentStatus::ACTUAL, { 1, 2 });
		search_server.AddDocument(1, "пушистый пЄс и модный ошейник"s, DocumentStatus::ACTUAL, { 1, 2 });
		for (const Document& document : search_server.FindTopDocuments("к\x12от"s)) {
			PrintDocument(document);
		}
	}
	catch (const invalid_argument& error) {
		cout << "Error: " << error.what() << endl;
	}

	// ѕроверка на "--" и "иван-чай"
	try {
		SearchServer search_server("и в на"s);
		search_server.AddDocument(0, "пушистый кот и модный ошейник"s, DocumentStatus::ACTUAL, { 1, 2 });
		search_server.AddDocument(1, "пушистый пЄс и модный иван-чай"s, DocumentStatus::ACTUAL, { 1, 2 });
		for (const Document& document : search_server.FindTopDocuments("иван-чай"s)) {
			PrintDocument(document);
		}
		for (const Document& document : search_server.FindTopDocuments("--кот"s)) {
			PrintDocument(document);
		}
	}
	catch (const invalid_argument& error) {
		cout << "Error: " << error.what() << endl;
	}

	// ѕроверка на пустоту
	try {
		SearchServer search_server("и в на"s);
		search_server.AddDocument(0, "пушистый кот и модный ошейник"s, DocumentStatus::ACTUAL, { 1, 2 });
		search_server.AddDocument(1, "пушистый пЄс и модный кот"s, DocumentStatus::ACTUAL, { 1, 2 });
		for (const Document& document : search_server.FindTopDocuments(""s)) {
			PrintDocument(document);
		}
	}
	catch (const invalid_argument& error) {
		cout << "Error: " << error.what() << endl;
	}

	// ѕроверка на пустое стоп-слово
	try {
		SearchServer search_server("и в на"s);
		search_server.AddDocument(0, "пушистый кот и модный ошейник"s, DocumentStatus::ACTUAL, { 1, 2 });
		search_server.AddDocument(1, "пушистый пЄс и модный кот"s, DocumentStatus::ACTUAL, { 1, 2 });
		for (const Document& document : search_server.FindTopDocuments("пушистый -"s)) {
			PrintDocument(document);
		}
	}
	catch (const invalid_argument& error) {
		cout << "Error: " << error.what() << endl;
	}

}