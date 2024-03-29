﻿// search_server_s1_t2_v2.cpp
#include <cassert>
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
        documents_.emplace(document_id,
            DocumentData{
                ComputeAverageRating(ratings),
                status
            });
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status = DocumentStatus::ACTUAL) const {
        return FindTopDocuments(raw_query, [&status](int document_id, DocumentStatus doc_status, int rating) { return doc_status == status; });
    }

    template <typename Container, typename KeyMapper>
    vector<Document> FindTopDocuments(const Container& raw_query, KeyMapper key_mapper) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, key_mapper);
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
        return {
            text,
            is_minus,
            IsStopWord(text)
        };
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

    template <typename KeyMapper>
    vector<Document> FindAllDocuments(const Query& query, KeyMapper key_mapper) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (key_mapper(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({
                document_id,
                relevance,
                documents_.at(document_id).rating
                });
        }
        return matched_documents;
    }
};


void PrintDocument(const Document& document) {
    cout << "{ "s
        << "document_id = "s << document.id << ", "s
        << "relevance = "s << document.relevance << ", "s
        << "rating = "s << document.rating
        << " }"s << endl;
}

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in"s).empty());
    }
}

/*
Разместите код остальных тестов здесь
*/

//Добавление документов. Добавленный документ должен находиться по поисковому запросу, который содержит слова из документа.
void TestAddDocument() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };
    SearchServer server;
    {
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }
    {
        server.AddDocument(43, "dog in the village"s, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("dog in the village"s);
        assert(found_docs.size() == 2);
        const Document& doc0 = found_docs[0];
        const Document& doc1 = found_docs[1];
        assert(doc0.id == 43);
        assert(doc1.id == doc_id);
    }
}

//Поддержка минус-слов. Документы, содержащие минус-слова поискового запроса, не должны включаться в результаты поиска.
void TestExcludeMinusWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };
    // Сначала убеждаемся, что находятся документы, не содержащие минус слова
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }

    // Затем убеждаемся, что поиск этого же документа, имеющего минус-слова,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, "cat in the -city"s, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in"s).empty());
    }
}

// Матчинг документов. При матчинге документа по поисковому запросу должны быть 
// возвращены все слова из поискового запроса, присутствующие в документе. Если 
// есть соответствие хотя бы по одному минус-слову, должен возвращаться пустой список слов.
void TestMatchingDocuments() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };
    vector<string> matches = { "cat"s, "in"s, "the"s, "city"s };
    sort(matches.begin(), matches.end());
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        auto temp = server.MatchDocument("cat in the city"s, doc_id);
        assert(matches == get<0>(temp));
    }
    sort(matches.begin(), matches.end());
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        auto temp = server.MatchDocument("cat in the -city"s, doc_id);
        matches.clear();
        assert(matches == get<0>(temp));
    }
}

// Сортировка найденных документов по релевантности.
void TestSortingOnRelevance() {
    SearchServer server;
    server.AddDocument(42, "cat in the city"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
    server.AddDocument(43, "dog in the village"s, DocumentStatus::ACTUAL, { 4, 5, 6 });
    server.AddDocument(45, "dog in the garden"s, DocumentStatus::ACTUAL, { 4, 5, 6 });
    const auto found_docs = server.FindTopDocuments("dog in the village"s);
    assert(found_docs.size() == 3);
    const Document& doc0 = found_docs[0];
    const Document& doc1 = found_docs[1];
    const Document& doc2 = found_docs[2];
    assert(doc0.relevance >= doc1.relevance >= doc2.relevance);
}

// Вычисление рейтинга документов. Рейтинг добавленного документа равен среднему арифметическому оценок документа.
void TestCalculateRating() {
    SearchServer server;
    server.AddDocument(45, "dog in the garden"s, DocumentStatus::ACTUAL, { 4, 5, 2 });
    const auto found_docs = server.FindTopDocuments("dog in the village"s);
    const Document& doc0 = found_docs[0];
    assert(doc0.rating == 3);
}

// Фильтрация результатов поиска с использованием предиката, задаваемого пользователем.
void TestPredicateFilter() {
    SearchServer server;
    server.AddDocument(42, "cat in the city"s, DocumentStatus::ACTUAL, { -1, -2, -3 });
    server.AddDocument(43, "dog in the village"s, DocumentStatus::ACTUAL, { -4, -5, -6 });
    server.AddDocument(45, "dog in the garden"s, DocumentStatus::ACTUAL, { 4, 5, 6 });
    const auto found_docs = server.FindTopDocuments("dog in the village"s, [](int document_id, DocumentStatus status, int rating) { return rating > 0; });
    assert(found_docs.size() == 1);
}

// Поиск документов, имеющих заданный статус.
void TestFindDocumentByStatus() {
    SearchServer server;
    server.AddDocument(42, "cat in the city"s, DocumentStatus::IRRELEVANT, { -1, -2, -3 });
    server.AddDocument(43, "dog in the village"s, DocumentStatus::IRRELEVANT, { -4, -5, -6 });
    server.AddDocument(45, "dog in the garden"s, DocumentStatus::ACTUAL, { 4, 5, 6 });
    const auto found_docs = server.FindTopDocuments("dog in the village"s, DocumentStatus::IRRELEVANT);
    assert(found_docs.size() == 2);
}

// Корректное вычисление релевантности найденных документов.
void TestRelevanceCalculation() {
    SearchServer server;
    double relevance_for_doc0 = log((3 * 1.0) / 1) * 0 + log((3 * 1.0) / 1) * (1.0 / 4.0) + log((3 * 1.0) / 2) * 0;
    server.AddDocument(45, "white cat and fashionable collar"s, DocumentStatus::ACTUAL, { 4, 5, 2 });
    server.AddDocument(45, "fluffy cat fluffy tail"s, DocumentStatus::ACTUAL, { 4, 5, 2 });
    server.AddDocument(45, "well-groomed dog expressive eyes"s, DocumentStatus::ACTUAL, { 4, 5, 2 });
    const auto found_docs = server.FindTopDocuments("fluffy well-groomed cat"s);
    const Document& doc0 = found_docs[0];
    assert(doc0.relevance - relevance_for_doc0 < 1e-6);
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    // Не забудьте вызывать остальные тесты здесь
    TestAddDocument();
    TestExcludeMinusWordsFromAddedDocumentContent();
    TestMatchingDocuments();
    TestSortingOnRelevance();
    TestCalculateRating();
    TestPredicateFilter();
    TestFindDocumentByStatus();
    TestRelevanceCalculation();
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}