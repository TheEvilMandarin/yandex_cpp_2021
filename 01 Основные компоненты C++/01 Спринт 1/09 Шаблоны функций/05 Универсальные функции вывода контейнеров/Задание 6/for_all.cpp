#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <windows.h>

using namespace std;

template <typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& container) {
    out << '(' << container.first << ", " << container.second << ')';
    return out;
}

template <typename Documents>
ostream& Print(ostream& out, Documents container) {
    for (auto element = container.begin(); element != prev(container.end()); element++) {
        out << *element << ", "s;
    }
    out << *container.rbegin();
    return out;
}

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& container) {
    out << '[';
    &Print(out, container);
    out << ']';
    return out;
}

template <typename T>
ostream& operator<<(ostream& out, const set<T>& container) {
    out << '{';
    &Print(out, container);
    out << '}';
    return out;
}

template <typename T1, typename T2>
ostream& operator <<(ostream& out, const map<T1, T2>& container)
{
    out << '<';
    &Print(out, container);
    out << '>';
    return out;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const set<string> cats = { "Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s };
    cout << cats << endl;

    const vector<int> ages = { 10, 5, 2, 12 };
    cout << ages << endl;

    const map<string, int> cat_ages = {
        {"Мурка"s, 10},
        {"Белка"s, 5},
        {"Георгий"s, 2},
        {"Рюрик"s, 12}
    };
    cout << cat_ages << endl;
}