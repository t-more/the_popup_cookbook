#include <bits/stdc++.h>
#include "line_segment.hpp"
using namespace std;
using namespace popup;

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

  size_t cases;
  cin >> cases;
  cout << setprecision(2) << fixed;
  while (cases--) {
      Point<2, double> p1, p2, p3, p4;
      cin >> p1[0] >> p1[1] >> p2[0] >> p2[1]
          >> p3[0] >> p3[1] >> p4[0] >> p4[1];
      LineSegment<double> a = {p1,p2}, b = {p3,p4};

      double ans = a.distance_to(b);
      cout << ans << endl;
  }

  return 0;
}
