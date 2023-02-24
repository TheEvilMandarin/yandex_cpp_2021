#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <windows.h>

using namespace std;

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& container) {
    for (auto element = container.begin(); element != prev(container.end()); element++) {
        out << *element << ", "s;
    }
    out << container.back();
    return out;
}

template <typename T>
ostream& operator<<(ostream& out, const set<T>& container) {
    for (auto element = container.begin(); element != prev(container.end()); element++) {
        out << *element << ", "s;
    }
    out << *container.rbegin();
    return out;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const vector<int> ages = { 10, 5, 2, 12 };
    cout << ages << endl;
    const set<string> cats = { "Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s };
    cout << cats << endl;
}