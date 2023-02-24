#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


int temp(string str){
	if(str[0]=='a' || str[0]=='A')
	return 1;
}
int CountStartsWithA(const vector<string>& xs) {
    // посчитайте число строк, начинающихся на букву A
    int count = 0;
    count = count_if(xs.begin(), xs.end(), temp);
    return count;
}

int main() {
    // не меняйте тело main
    cout << CountStartsWithA({"And"s, "another"s, "one"s, "gone"s, "another"s, "one"s "bites"s, "the"s, "dust"s});
		return 0;
