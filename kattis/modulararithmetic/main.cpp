#include <bits/stdc++.h>
#include "modular.hpp"
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
#define EPS 10e-9

//#define DEBUG
#ifdef DEBUG
#define D(...) fprintf(stderr, __VA_ARGS__);
#else
#define D(...)
#endif

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
    cin.sync_with_stdio(0);
    cin.tie(0);
    
    {
        auto [gcd, x, y] = 
            popup::extended_euclidean<uint64_t, int64_t>((uint64_t)999, (uint64_t)1000);
        D("%lu %ld %ld\n", gcd, x, y);
        auto opt = popup::mod_inverse<uint64_t, int64_t>((uint64_t)999, (uint64_t)1000);
        if(opt.has_value()) {
            D("%lu\n", opt.value());
        } else {
            D("fuck inverses\n");
        }

    }
    uint64_t n, t;
    while(cin >> n >> t && n != 0 && t != 0) {
        D("n: %lu t: %lu\n", n, t);
        uint64_t a, b;
        char op;
        for (int i = 0; i < (int)t; i++) {
            cin >> a >> op >> b;
            D("a: %lu op: %c b: %lu\n", a, op, b);
            int64_t res;
            switch(op) {
            case '+':
                res = popup::add_mod<uint64_t>(a, b, n);
                break;
            case '-':
                res = popup::sub_mod<uint64_t>(a, b, n);
                break;
            case '/':
                {
                    auto div_res = popup::div_mod<uint64_t, int64_t>(a, b, n);
                    if(div_res.has_value()) {
                        res = div_res.value();
                    } else {
                        res = -1;
                    }
                }
                break;
            case '*':
                res = popup::mul_mod<uint64_t>(a, b, n);
                break;
            }

            cout << res << endl;
        }
    }

    return 0;
}
