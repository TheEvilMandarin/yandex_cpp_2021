#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

const int SAMPLE_COUNT = 5;

vector<int> CountNamesLong(const set<string>& storage, const vector<string>& candidates) {
    vector<int> output;
    for (auto& name : candidates) {
        output.push_back(count(storage.begin(), storage.end(), name));
    }
    return output;
}

vector<int> CountNames(const set<string>& storage, const vector<string>& candidates) {
    vector<int> output;
    for (auto& name : candidates) {
        if(storage.count(name)==0)
            output.push_back(0);
        else
            output.push_back(1);
    }
    return output;
}

int main() {
    set<string> s;
    vector<string> v;
    string stra = ""s;
    string strb = ""s;

    for (int j = 0; j < 20; ++j) {
        s.insert(stra);
        stra += "a"s;
        if (j % 2 == 0) {
            v.push_back(strb);
            strb += "b"s;
        }
        else {
            v.push_back(stra);
        }
    }
    cout << "Testing slow version" << endl;
    for (int i = 0; i < CountNames(s, v).size(); ++i) {
        cout << CountNames(s, v).at(i)<<" ";
        /*auto begin = chrono::steady_clock::now();
        CountNamesLong(s, v);
        auto end = chrono::steady_clock::now();
        cout << "Time difference Long = "s << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs]"s << endl;*/
    }
}