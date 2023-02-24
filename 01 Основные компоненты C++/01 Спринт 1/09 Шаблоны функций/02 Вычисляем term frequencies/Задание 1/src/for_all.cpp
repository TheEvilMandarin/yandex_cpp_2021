#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

map<string, int> ComputeTermFreqs(const vector<string>& terms) {
    // реализуйте функцию
    map<string, int> res;
    vector<string>::const_iterator it;
    for (it = terms.begin(); it < terms.end(); it++) {
        res[*it]++;
    }
    return res;
}

int main() {
    const vector<string> terms = { "first"s, "time"s, "first"s, "class"s };
    for (const auto& [term, freq] : ComputeTermFreqs(terms)) {
        cout << term << " x "s << freq << endl;
    }
    // вывод:
    // class x 1
    // first x 2
    // time x 1
}