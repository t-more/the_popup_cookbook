#include <bits/stdc++.h>
#include "numeric.hpp"

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
  uint64_t n, q;
  cin >> n >> q;
  SieveCache cache(n);

  cout << cache.primes_found() << "\n";

  while (q--) {
      uint64_t num;
      cin >> num;
      cout << (int)cache.is_prime(num) << "\n";
  }

  return 0;
}
