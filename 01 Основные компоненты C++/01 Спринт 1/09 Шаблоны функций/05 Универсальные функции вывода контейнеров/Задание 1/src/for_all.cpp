#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& container) {
    for (const T& element : container) {
        out << element << " "s;
    }
    return out;
}

int main() {
    const vector<int> ages = { 10, 5, 2, 12 };
    cout << ages << endl;
}