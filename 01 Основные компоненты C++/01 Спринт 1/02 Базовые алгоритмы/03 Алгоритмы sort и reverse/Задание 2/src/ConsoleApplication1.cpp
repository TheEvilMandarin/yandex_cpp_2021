#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

pair<bool, double> CalcMedian(vector<double> samples) {
    // верните {true, медиана}, если она существует,
    // то есть вектор непустой,
    // иначе - {false, 0}
    // обратите внимание - вектор принимаем по значению,
    // так как его придётся немного подпортить, чтобы вернуть ответ
    if (samples.size() == 0)
        return make_pair(false, 0);
    auto m = samples.begin() + samples.size() / 2;
    nth_element(samples.begin(), m, samples.end());
    if (samples.size() %2 == 0)
        return make_pair(true, (samples[samples.size() / 2]+ samples[(samples.size()-1) / 2])/2);
    return make_pair(true, samples[samples.size() / 2]);
}

int main() {
    int size;
    cin >> size;

    vector<double> samples;
    for (int i = 0; i < size; ++i) {
        double sample;
        cin >> sample;
        samples.push_back(sample);
    }

    pair<bool, double> result = CalcMedian(samples);
    if (result.first) {
        cout << result.second << endl;
    }
    else {
        cout << "Empty vector"s << endl;
    }
}