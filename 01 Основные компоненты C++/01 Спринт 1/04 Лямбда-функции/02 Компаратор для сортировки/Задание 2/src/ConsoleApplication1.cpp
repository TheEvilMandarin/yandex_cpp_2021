#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    vector<string> arr;
    string str;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> str;
        arr.push_back(str);
    }
    sort(arr.begin(), arr.end(),
        [](string lha, string rha) {
            return lexicographical_compare(lha.begin(), lha.end(),
                rha.begin(), rha.end(), [](char c1, char c2) { return tolower(c1) < tolower(c2); });
    });
    for (int i = 0; i < n; i++)
       cout << arr.at(i) << " ";
    cout << endl;
    return 0;
}