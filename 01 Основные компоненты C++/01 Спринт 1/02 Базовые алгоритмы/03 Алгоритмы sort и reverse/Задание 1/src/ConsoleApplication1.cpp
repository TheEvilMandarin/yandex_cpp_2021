#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int count;
    cin >> count;
    vector<pair<int, string>> vec;
    for (int i = 0; i < count; ++i) {
        string name;
        int age;
        cin >> name >> age;
        // сохраните в вектор пар
        vec.push_back(make_pair(age, name));
    }
    sort(vec.begin(), vec.end());
    reverse(vec.begin(), vec.end());
    for (const auto& element : vec)
        cout << element.second << " " << element.first << endl;
    // выведите только имена в порядке убывания возраста
    // Jack
    // John
    // ...
}