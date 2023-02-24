#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>

using namespace std;

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& container) {
    for (auto element = container.begin(); element != prev(container.end()); element++) {
        out << *element << ", "s;
    }
    out << container.back();
    return out;
}

//template <typename T>
//ostream& operator<<(ostream& out, const set<T>& container) {
//    string str;
//    for (int i = 0; i < container.size() - 1; i++) {
//        str += to_string(container. + ", "s;
//    }
//    str += to_string(container.back());
//    out << str;
//    return out;
//}

int main() {
    const vector<int> ages = { 10, 5, 2, 12 };
    cout << ages << endl;
    //const set<string> cats = { "Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s };
    //cout << cats << endl;
}