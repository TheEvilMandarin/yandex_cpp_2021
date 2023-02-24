﻿#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

int main() {
    const auto min = numeric_limits<int64_t>::min();// минимальное значение типа int64_t
    const auto max = numeric_limits<uint64_t>::max();// максимальное значение типа uint64_t
    cout << min << endl;
    cout << max << endl;
    //cout << min + max << endl;
    cout << min * 2 << endl;
    cout << max * 2 << endl;
        // выведите 5 чисел
}