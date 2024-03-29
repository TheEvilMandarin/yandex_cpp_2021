﻿#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <Windows.h>

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

struct Query {
	vector<string> plus;
	vector<string> minus;
};

class SearchServer {
public:
	void SetStopWords(const string& text) {
		for (const string& word : SplitIntoWords(text)) {
			stop_words_.insert(word);
		}
	}

	void AddDocument(int document_id, const string& document) {
		for (const string& word : SplitIntoWordsNoStop(document)) {
			word_to_documents_[word].insert(document_id);
		}
	}

	vector<Document> FindTopDocuments(const string& query) const {
		auto matched_documents = FindAllDocuments(query);

		sort(
			matched_documents.begin(),
			matched_documents.end(),
			[](const Document& lhs, const Document& rhs) {
				return lhs.relevance > rhs.relevance;
			}
		);
		if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
			matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		}
		return matched_documents;
	}

private:
	map<string, set<int>> word_to_documents_;
	set<string> stop_words_;

	vector<string> SplitIntoWordsNoStop(const string& text) const {
		vector<string> words;
		for (const string& word : SplitIntoWords(text)) {
			if (stop_words_.count(word) == 0) {
				words.push_back(word);
			}
		}
		return words;
	}

	Query ParseQuery(const string& text) const {
		Query words;
		for (const string& word : SplitIntoWords(text)) {
			if (stop_words_.count(word) == 0 && word[0]!='-') {
				words.plus.push_back(word);
			}
			else if (stop_words_.count(word) == 0) {
				words.minus.push_back(word.substr(1));
			}
		}
		return words;
	}

	vector<Document> FindAllDocuments(const string& query) const {
		const Query words = ParseQuery(query);
		const vector<string> query_words = words.plus;
		const vector<string> query_words_minus = words.minus;
		bool del = false;
		map<int, int> document_to_relevance;
		for (const string& word : query_words) {
			if (word_to_documents_.count(word) == 0) {
				continue;
			}
			for (const int document_id : word_to_documents_.at(word)) {
				++document_to_relevance[document_id];
			}
		}
		vector<Document> matched_documents;
		for (auto [document_id, relevance] : document_to_relevance) {
			matched_documents.push_back({ document_id, relevance });
		}

		for (const string& word_minus : query_words_minus) {
			map<string, set<int>>::const_iterator it = word_to_documents_.begin();
			while(it != word_to_documents_.end()) {
				if (it->first == word_minus) {
					set<int>::iterator it_id = it->second.begin();
					while (it_id != it->second.end()) {
						matched_documents.erase(
							remove_if(matched_documents.begin(), matched_documents.end(), [&](Document const& doc) {
								return doc.id == *it_id;
								}),
							matched_documents.end());
						it_id++;
					}
				}
				it++;
			}
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
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	const SearchServer search_server = CreateSearchServer();

	const string query = ReadLine();
	for (auto [document_id, relevance] : search_server.FindTopDocuments(query)) {
		cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << endl;
	}
}