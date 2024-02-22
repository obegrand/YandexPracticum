//for YandexPracticum from obegrand aka Andrey Melnikov
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

template <typename T>
ostream& operator<<(ostream& output, const vector<T>& items) {
	output << "["s;
	bool first_item = true;
	for (const T& item : items) {
		if (!first_item) {
			output << ", "s;
		}
		output << item;
		first_item = false;
	}
	output << "]"s;
	return output;
}

template <typename T>
ostream& operator<<(ostream& output, const set<T>& items) {
	output << "{"s;
	bool first_item = true;
	for (const T& item : items) {
		if (!first_item) {
			output << ", "s;
		}
		output << item;
		first_item = false;
	}
	output << "}"s;
	return output;
}

template <typename K, typename V>
ostream& operator<<(ostream& output, const map<K, V>& items) {
	output << "{"s;
	bool first_item = true;
	for (const auto& [key, value] : items) {
		if (!first_item) {
			output << ", "s;
		}
		output << key << ": "s << value;
		first_item = false;
	}
	output << "}"s;
	return output;
}

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
	int id;
	double relevance;
	int rating;
};

enum class DocumentStatus {
	ACTUAL,
	IRRELEVANT,
	BANNED,
	REMOVED,
};

class SearchServer {
public:
	void SetStopWords(const string& text) {
		for (const string& word : SplitIntoWords(text)) {
			stop_words_.insert(word);
		}
	}

	void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
		const vector<string> words = SplitIntoWordsNoStop(document);
		const double inv_word_count = 1.0 / words.size();
		for (const string& word : words) {
			word_to_document_freqs_[word][document_id] += inv_word_count;
		}
		documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });
	}

	template<typename DocumentPredicate>
	vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate predicate) const {
		const Query query = ParseQuery(raw_query);
		auto matched_documents = FindAllDocuments(query, predicate);

		sort(matched_documents.begin(), matched_documents.end(),
			[](const Document& lhs, const Document& rhs) {
				if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
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

	vector<Document> FindTopDocuments(const string& raw_query) const {
		return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
	}

	vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
		return FindTopDocuments(raw_query,
			[&status](int document_id, DocumentStatus new_status, int rating) {
				return new_status == status;
			});
	}

	int GetDocumentCount() const {
		return documents_.size();
	}

	tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
		const Query query = ParseQuery(raw_query);
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
		return { matched_words, documents_.at(document_id).status };
	}

private:
	struct DocumentData {
		int rating;
		DocumentStatus status;
	};

	set<string> stop_words_;
	map<string, map<int, double>> word_to_document_freqs_;
	map<int, DocumentData> documents_;

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
		// Word shouldn't be empty
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

	template<typename DocumentPredicate>
	vector<Document> FindAllDocuments(const Query& query, DocumentPredicate predicate) const {
		map<int, double> document_to_relevance;
		for (const string& word : query.plus_words) {
			if (word_to_document_freqs_.count(word) == 0) {
				continue;
			}
			for (const auto& [document_id, term_freq] : word_to_document_freqs_.at(word)) {
				if (predicate(document_id, documents_.at(document_id).status, documents_.at(document_id).rating)) {
					const double IDF = log(static_cast<double>(documents_.size()) / word_to_document_freqs_.at(word).size());
					document_to_relevance[document_id] += IDF * term_freq;
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

void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line, const string& hint) {
	if (!value) {
		cerr << file << "("s << line << "): "s << func << ": "s;
		cerr << "Assert("s << expr_str << ") failed."s;
		if (!hint.empty()) {
			cerr << " Hint: "s << hint;
		}
		cerr << endl;
		abort();
	}
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file, const string& func, unsigned line, const string& hint) {
	if (t != u) {
		cout << boolalpha;
		cout << file << "("s << line << "): "s << func << ": "s;
		cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
		cout << t << " != "s << u << "."s;
		if (!hint.empty()) {
			cout << " Hint: "s << hint;
		}
		cout << endl;
		abort();
	}
}

template <typename TestFunc>
void RunTestImpl(const TestFunc& func, const string& test_name) {
	func();
	cerr << test_name << " OK"s << endl;
}

#define ASSERT(expr) AssertImpl((expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl((expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define RUN_TEST(func) RunTestImpl(func, #func) 

void TestExcludeStopWordsFromAddedDocumentContent() {
	const int doc_id = 42;
	const string content = "cat in the city"s;
	const vector<int> ratings = { 1, 2, 3 };

	{
		SearchServer server;
		server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
		const auto found_docs = server.FindTopDocuments("in"s);
		ASSERT_EQUAL(found_docs.size(), 1u);
		const Document& doc0 = found_docs[0];
		ASSERT_EQUAL(doc0.id, doc_id);
	}



	{
		SearchServer server;
		server.SetStopWords("in the"s);
		server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
		ASSERT_HINT(server.FindTopDocuments("in"s).empty(), "Stop words must be excluded from documents"s);
	}
}

// Добавление документов. Добавленный документ должен находиться по поисковому запросу, который содержит слова из документа. 
void TestFindAddedDocumentByDocumentWord() {
	const int doc_id = 42;
	const string content = "cat in the city"s;
	const vector<int> ratings = { 1, 2, 3 };

	{
		SearchServer server;
		ASSERT_EQUAL(server.GetDocumentCount(), 0);
		server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
		ASSERT_EQUAL(server.GetDocumentCount(), 1);
		server.AddDocument(doc_id + 1, "black dog fluffy tail", DocumentStatus::ACTUAL, ratings);
		ASSERT_EQUAL(server.GetDocumentCount(), 2);
		const auto found_docs = server.FindTopDocuments("cat"s);
		ASSERT_EQUAL(found_docs.size(), 1u);
		const Document& doc0 = found_docs[0];
		ASSERT_EQUAL(doc0.id, doc_id);
	}
}

// Поддержка минус-слов. Документы, содержащие минус-слова из поискового запроса, не должны включаться в результаты поиска. 
void TestExcludeDocumentsWithMinusWordsFromResults() {
	SearchServer server;
	server.AddDocument(101, "fluffy cat fluffy tail"s, DocumentStatus::ACTUAL, { 1,2,3 });
	server.AddDocument(102, "fluffy red dog "s, DocumentStatus::ACTUAL, { 1,2,3 });

	{
		const auto found_docs = server.FindTopDocuments("fluffy -dog"s);
		ASSERT_EQUAL(found_docs.size(), 1u);
		ASSERT_EQUAL(found_docs[0].id, 101);
	}

	{
		const auto found_docs = server.FindTopDocuments("fluffy -cat"s);
		ASSERT_EQUAL(found_docs.size(), 1u);
		ASSERT_EQUAL(found_docs[0].id, 102);
	}

}

// Соответствие документов поисковому запросу. 
void TestMatchedDocuments() {
	SearchServer server;
	server.SetStopWords("and in on"s);
	DocumentStatus stat = DocumentStatus::IRRELEVANT;
	server.AddDocument(100, "fluffy cat and black dog in a collar"s, stat, { 1, 2, 3 });

	{
		const auto [matched_words, status] = server.MatchDocument("dog and cat"s, 100);
		const vector<string> expected_result = { "cat"s, "dog"s };
		ASSERT_EQUAL(expected_result, matched_words);
		ASSERT_EQUAL(static_cast<int>(stat), static_cast<int>(status));
	}

	{
		const auto [matched_words, status] = server.MatchDocument("dog and -cat"s, 100);
		const vector<string> expected_result = {}; // пустой результат поскольку есть минус-слово 
		ASSERT_EQUAL(expected_result, matched_words);
		ASSERT(matched_words.empty());
	}
}

// Сортировка найденных документов по релевантности. 
void TestSortResultsByRelevance() {
	SearchServer server;
	server.AddDocument(100, "fluffy cat fluffy tail"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
	server.AddDocument(101, "fluffy dog"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
	server.AddDocument(102, "dog leather collar"s, DocumentStatus::ACTUAL, { 1, 2, 3 });

	{
		const auto found_docs = server.FindTopDocuments("fluffy cat"s);
		ASSERT_EQUAL(found_docs.size(), 2u);
		for (size_t i = 1; i < found_docs.size(); i++) {
			ASSERT(found_docs[i - 1].relevance >= found_docs[i].relevance);
		}
	}
}

// Вычисление рейтинга документов. 
void TestCalculateDocumentRating() {
	SearchServer server;
	const vector<int> ratings = { 10, 11, 3 };
	const int average = (10 + 11 + 3) / 3;
	server.AddDocument(0, "fluffy cat fluffy tail"s, DocumentStatus::ACTUAL, ratings);

	{
		const auto found_docs = server.FindTopDocuments("fluffy cat"s);
		ASSERT_EQUAL(found_docs.size(), 1u);
		ASSERT_EQUAL(found_docs[0].rating, average);
	}
}

// Фильтрация результатов поиска с использованием предиката. 
void TestDocumentSearchByPredicate() {
	SearchServer server;
	server.AddDocument(100, "cat in the city"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
	server.AddDocument(101, "dog in the town"s, DocumentStatus::IRRELEVANT, { -1, -2, -3 });
	server.AddDocument(102, "dog and rabbit in the town"s, DocumentStatus::ACTUAL, { -4, -5, -6 });
	const auto found_docs = server.FindTopDocuments("in the cat"s, [](int document_id, DocumentStatus status, int rating) { return rating > 0; });

	{
		ASSERT_HINT(found_docs[0].id == 100, "Wrong predicate");
	}
}

// Поиск документов, имеющих заданный статус. 
void TestDocumentSearchByStatus() {
	const int doc_id1 = 42;
	const int doc_id2 = 43;
	const int doc_id3 = 44;
	const string content1 = "cat in the city"s;
	const string content2 = "cat in the town"s;
	const string content3 = "cat in the town or city"s;
	const vector<int> ratings = { 1, 2, 3 };
	SearchServer server;
	server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings);
	server.AddDocument(doc_id2, content2, DocumentStatus::IRRELEVANT, ratings);
	server.AddDocument(doc_id3, content3, DocumentStatus::IRRELEVANT, ratings);
	const auto found_docs = server.FindTopDocuments("in the cat"s, DocumentStatus::IRRELEVANT);

	{
		ASSERT_HINT(found_docs[0].id == doc_id2, "Wrong status");
		ASSERT_HINT(found_docs[1].id == doc_id3, "Wrong status");
		ASSERT_HINT(found_docs.size() == 2, "Wrong status request");
	}
}

// Корректное вычисление релевантности найденных документов. 
void TestCalculateRelevance() {
	SearchServer server;
	server.AddDocument(100, "white cat with new ring"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
	server.AddDocument(101, "fluffy cat fluffy tail"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
	server.AddDocument(102, "good dog big eyes"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
	const auto found_docs = server.FindTopDocuments("fluffy good cat"s);
	double relevance_query = log((3 * 1.0) / 1) * (2.0 / 4.0) + log((3 * 1.0) / 2) * (1.0 / 4.0);

	{
		ASSERT_HINT(fabs(found_docs[0].relevance - relevance_query) < 1e-6, "Wrong calculation relevance");
	}
}

// Функция TestSearchServer является точкой входа для запуска тестов 
void TestSearchServer() {
	RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
	RUN_TEST(TestFindAddedDocumentByDocumentWord);
	RUN_TEST(TestExcludeDocumentsWithMinusWordsFromResults);
	RUN_TEST(TestMatchedDocuments);
	RUN_TEST(TestSortResultsByRelevance);
	RUN_TEST(TestCalculateDocumentRating);
	RUN_TEST(TestDocumentSearchByPredicate);
	RUN_TEST(TestDocumentSearchByStatus);
	RUN_TEST(TestCalculateRelevance);
}

int main() {
	TestSearchServer();
	// Если вы видите эту строку, значит все тесты прошли успешно
	cout << "Search server testing finished"s << endl;
}