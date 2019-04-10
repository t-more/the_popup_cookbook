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
        cout << lr.get(n, m) << endl;
    }

    return 0;
}
