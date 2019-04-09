#include <bits/stdc++.h>
#include "matrix.hpp"
#include "linear_recurrence.hpp"
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

    size_t N;
    cin >> N;
    vector<int64_t> coefficient(N+1);
    vector<int64_t> init_values(N);
    for(auto &i : coefficient) {
        cin >> i;
    }
    for(auto &i : init_values) {
        cin >> i;
    }
    popup::LinearRecurrence lr(N, coefficient, init_values);
    size_t Q;
    cin >> Q;
    while(Q--) {
        size_t n;
        int64_t m;
        cin >> n >> m;
        cout << "n: " << n << " m: " << m << endl;
        cout << lr.get(n, m) << endl;
    }

    cout << endl << endl;

    Matrix<uint64_t> m1 = {  
        0, 1, 0,  
        9, 7, 1,  
        0, 0, 1 
    };
    vector<uint64_t> v = {36713, 5637282, 5};
    size_t t1 = 3781, t2 = 23;
    vector<uint64_t> t = mul_mod(m1.pow_modulus(1, 10000), v, 10000);
    cout << "t: " << endl;
    for(auto i : t) {
        cout << i << " ";
    }
    cout << endl;
    m1 = m1.pow_modulus(t1, t2);
    cout << m1 << endl<< endl;
    vector<uint64_t> v2 = mul_mod(m1, v, 23);
    for(auto i : v2) {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}
