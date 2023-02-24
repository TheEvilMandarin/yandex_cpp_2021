#include <iostream>
#include <set>
#include <numeric>

using namespace std;

string AddStopWords(const string& query, const set<string>& stop_words) {
    string s;
    s = accumulate(begin(stop_words), end(stop_words), string(), [](string lhs, const string& rhs) { return lhs.empty() ? rhs : lhs + ' ' + rhs; });
    string rez = query + s;
    return rez;
}

int main() {
    cout << AddStopWords("some tasty oranges", { "-of"s, "-in"s }) << endl;
    return 0;
}