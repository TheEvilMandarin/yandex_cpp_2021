#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main() {
    string str, str2, word;
    getline(cin, str);
    istringstream iss(str);
    vector<string> v1, v2;
    while (iss >> word) {
        v1.push_back(word);
        reverse(word.begin(), word.end());
        v2.push_back(word);
    }
    reverse(v2.begin(), v2.end());
    str = str2 = "";
    for (auto it : v1)
        str += it;
    for (auto it : v2)
        str2 += it;
    if (str == str2 && str!="")
        cout << "palindrome";
    else
        cout << "not a palindrome";
    return 0;
}
