#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin, x.end()
#define sx(x) (int)(x).size()
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;


using f = __float128;
long double masin(long double n) {
    return asin(n);
}
int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);
  cin.exceptions(cin.failbit);

  cout << setprecision(9);
  cout << fixed;
  double Li, ni, Ci;

  while (cin >> Li >> ni >> Ci && Li != -1) {
      f L = Li, n = ni, C = Ci;
      if (C < 1e-9 || n < 1e-9) {
          cout << 0.0 << endl;
          continue;
      }
      auto arc_len = (1.0 + n * C) * L;

      f h = L / 4.0;

      f h_err = 1.0;
      while ( abs(h_err) >  1e-8) {
          f s = masin(L / (h + (L * L) / ((long double)4.0 * h))) * (h + (L * L) / (4 * h));
          auto s_err = s - arc_len;
          auto hp = h - s_err / 100.0;
          h_err = h - hp;
          h = hp;
          //cerr << h << " " << arc_len << " " << s  << " " << err << endl;
      }
      cout << (double)h << endl;
      //cout << endl;
  }
}
