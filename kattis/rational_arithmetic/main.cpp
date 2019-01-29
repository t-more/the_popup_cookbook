#include <iostream>
#include "rational.hpp"

using namespace std;

int main() {
    int cases;
    cin >> cases;
    
    while(cases-- > 0) {
        int64_t n1, d1, n2, d2;
        char op;
        cin >> n1 >> d1 >> op >> n2 >> d2;
        popup::Rational<int64_t> r1(n1, d1);
        popup::Rational<int64_t> r2(n2, d2);
        
        switch(op) {
            case '+':
                r1 += r2;
                break;
            case '-':
                r1 -= r2;
                break;
            case '*':
                r1 *= r2;
                break;
            case '/':
                r1 /= r2;
                break;
        }

        cout << r1.numenator() << " / " << r1.denominator() << "\n";
    }

    return 0;
}
