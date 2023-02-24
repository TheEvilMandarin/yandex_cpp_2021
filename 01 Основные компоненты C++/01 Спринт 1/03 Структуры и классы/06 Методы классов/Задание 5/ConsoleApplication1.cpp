// final080.cpp с Document вместо пары

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;


const int MAX_RESULT_DOCUMENT_COUNT = 5;


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
			words.push_back(word);
			word = "";
		}
		else {
			word += c;
		}
	}
	words.push_back(word);

	return words;
}

struct Document {
	int id;
	int relevance;
};

bool HasDocumentGreaterRelevance(const Document& lhs, const Document& rhs) {
	return lhs.relevance > rhs.relevance;
}

class SearchServer {
private:
	set<string> stop_words;
	map<string, set<int>> word_to_documents;
	vector<string> SplitIntoWordsNoStop(const string& text) {
		vector<string> words;
		for (const string& word : SplitIntoWords(text)) {
			if (stop_words.count(word) == 0) {
				words.push_back(word);
			}
		}
		return words;
	}

	vector<Document> FindAllDocuments(const string& query) {
		const vector<string> query_words = SplitIntoWordsNoStop(query);
		map<int, int> document_to_relevance;
		for (const string& word : query_words) {
			if (word_to_documents.count(word) == 0) {
				continue;
			}
			for (const int document_id : word_to_documents.at(word)) {
				++document_to_relevance[document_id];
			}
		}

		vector<Document> matched_documents;
		for (auto [document_id, relevance] : document_to_relevance) {
			matched_documents.push_back({ document_id, relevance });
		}

		return matched_documents;
	}

public: void AddDocument(int document_id,
		const string& document) {
		for (const string& word : SplitIntoWordsNoStop(document)) {
			word_to_documents[word].insert(document_id);
		}
	}

	  set<string> SetStopWords(const string& text) {
		  for (const string& word : SplitIntoWords(text)) {
			  stop_words.insert(word);
		  }
		  return stop_words;
	  }

	  vector<Document> FindTopDocuments(const string& query) {

		  auto matched_documents = FindAllDocuments(query);

		  sort(matched_documents.begin(), matched_documents.end(), HasDocumentGreaterRelevance);
		  if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
			  matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		  }
		  return matched_documents;
	  }
};


SearchServer CreateSearchServer() {
	SearchServer search_server;
	search_server.SetStopWords(ReadLine());
	const int document_count = ReadLineWithNumber();
	for (int document_id = 0; document_id < document_count; ++document_id) {
		search_server.AddDocument(document_id, ReadLine());
	}
	return search_server;
}

int main() {
	SearchServer search_server = CreateSearchServer();
	const string query = ReadLine();
	//search_server.FindTopDocuments(query);
	for (auto [document_id, relevance] : search_server.FindTopDocuments(query)) {
		cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << endl;
	}
	//const string stop_words_joined = ReadLine();
	//const set<string> stop_words = ParseStopWords(stop_words_joined);

	//// Read documents
	//map<string, set<int>> word_to_documents;
	//const int document_count = ReadLineWithNumber();
	//for (int document_id = 0; document_id < document_count; ++document_id) {
	//	AddDocument(word_to_documents, stop_words, document_id, ReadLine());
	//}

	//const string query = ReadLine();
	//for (auto [document_id, relevance] : FindTopDocuments(word_to_documents, stop_words, query)) {
	//	cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << endl;
	//}
}