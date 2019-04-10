#include <bits/stdc++.h>
#include "modular.hpp"
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

  size_t num_cases;
  cin >>  num_cases;
  while (num_cases--) {
      int64_t a1, m1, a2, m2;
      cin >> a1 >> m1 >> a2 >> m2;
      auto res = popup::general_chinese_rest_theorem(a1, m1, a2, m2);
      auto [gcd, x, y] = popup::extended_euclidean<uint64_t, int64_t>(m1, m2);
      if(res.has_value()) {
        cout << res.value() << " " << (m1 * m2)/gcd << "\n";
      } else {
        cout << "no solution\n";
      }
  }
  return 0;
}
