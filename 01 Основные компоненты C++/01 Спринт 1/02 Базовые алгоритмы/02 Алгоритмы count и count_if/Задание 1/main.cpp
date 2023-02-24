#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void PrintWorryCount(const vector<bool>& is_nervous) {
    // подсчитываем количество элементов в векторе is_nervous, равных true
    
    cout << count(is_nervous.begin(), is_nervous.end(), true) << endl;
}

int main() {
    // не меняйте содержимое функции main
    PrintWorryCount({true, true, false, true});
    return 0;
