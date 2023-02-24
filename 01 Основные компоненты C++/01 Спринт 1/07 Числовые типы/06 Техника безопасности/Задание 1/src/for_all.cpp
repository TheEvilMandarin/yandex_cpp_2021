#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Document {
    int id;
    int rating;
};

void PrintDocuments(vector<Document> documents, size_t skip_start, size_t skip_finish) {
    // отсортировать документы и вывести не все
    sort(documents.begin(), documents.end(),
        [](const Document& lhs, const Document& rhs) {
            return lhs.rating > rhs.rating;
        });
    for (size_t i = skip_start; i < documents.size() - skip_finish; i++) {
        cout << "{ id: " << documents[i].id << ", rating: " << documents[i].rating << " }" << endl;
    }
    /*for (auto [id, rating] : documents)
        cout << "{ id: " << id << ", rating: " << rating << " }" << endl;*/
}

int main() {
    PrintDocuments(
        {
            {100, 5},
            {101, 7},
            {102, -4},
            {103, 9},
            {104, 1}
        },
        1,
        2
    );
}
