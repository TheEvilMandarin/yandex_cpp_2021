#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <map>

using namespace std;

int CountAndAddNewDogs(const vector<string>& new_dogs,
    const map<string, int>& max_amount, map<string, int>& shelter) {
    //...
    return count_if(
        new_dogs.begin(), new_dogs.end(),
        [&max_amount, &shelter, &new_dogs](const string& dog) {
            int kol = 0;
            auto it_shelter = shelter.find(dog);
            auto it_MaxAmount = max_amount.find(dog);
            auto old_kol = it_shelter->second;
            if (it_MaxAmount->second - old_kol > 0) {
                kol++;
                shelter.erase(dog);
                shelter.insert(make_pair(dog, ++old_kol));
            }
            return kol;
        }

    );
}

int main() {
    map<string, int> shelter{
    {"landseer"s, 1},
    {"otterhound"s, 2},
    {"pekingese"s, 2},
    {"pointer"s, 3}
    };
    const map<string, int> max_amount{
        {"landseer"s, 2},
        {"otterhound"s, 3},
        {"pekingese"s, 4},
        {"pointer"s, 7}
    };
    const vector<string> new_dogs{
        "landseer"s,
        "otterhound"s,
        "otterhound"s,
        "otterhound"s,
        "pointer"s
    };
    cout << CountAndAddNewDogs(new_dogs, max_amount, shelter) << endl;
    return 0;
}