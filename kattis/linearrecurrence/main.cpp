#include <bits/stdc++.h>
#include "matrix.hpp"
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
#define EPS 10e-9

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
    cin.sync_with_stdio(0);
    cin.tie(0);

    Matrix<int64_t> m({1,2,3,4});
    cout << m << endl;
    cout << endl;
    cout << m.pow(8) << endl;

    return 0;
}
