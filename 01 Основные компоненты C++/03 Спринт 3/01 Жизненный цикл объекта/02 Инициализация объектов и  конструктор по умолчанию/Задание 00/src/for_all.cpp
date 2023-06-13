#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

class Rational {
public:
    Rational() {
        numerator_ = 0;
        denominator_ = 1;
    }
    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

    void SetNumerator(int numerator) {
        numerator_ = numerator;
    }

    void SetDenominator(int denominator) {
        if (denominator != 0) {
            denominator_ = denominator;
        }
    }

private:
    int numerator_;
    int denominator_;
};

int main() {
    Rational r;
    cout << r.Numerator() << "/" << r.Denominator() << endl;
}