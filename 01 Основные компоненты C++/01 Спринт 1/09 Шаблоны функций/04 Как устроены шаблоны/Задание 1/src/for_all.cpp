#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

template <typename Documents, typename Term>
vector <double> ComputeTfIdfs(const  Documents& documents, const  Term& term) {
    vector<double> tf_idfs;
    double idf = log(documents.size() / static_cast<double>(count_if(documents.begin(), documents.end(), [term](vector<string> document) {
        return count(document.begin(), document.end(), term) > 0;
        })));
    for (const auto& document : documents){
        double tf = count(document.begin(), document.end(), term) / static_cast<double>(document.size());
        tf_idfs.push_back(tf * idf);
    }
    return tf_idfs;
}

int main() {
    const vector<vector<string>> documents = {
        {"белый"s, "кот"s, "и"s, "модный"s, "ошейник"s},
        {"пушистый"s, "кот"s, "пушистый"s, "хвост"s},
        {"ухоженный"s, "пёс"s, "выразительные"s, "глаза"s},
    };
    const auto& tf_idfs = ComputeTfIdfs(documents, "кот"s);
    for (const double tf_idf : tf_idfs) {
        cout << tf_idf << " "s;
    }
    cout << endl;
}