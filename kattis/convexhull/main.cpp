#include <bits/stdc++.h>
#include "convex_hull.hpp"
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

  size_t test_case;
  vector<popup::Point2<int>> points;
  while (cin >> test_case && test_case != 0) {
      points.clear();
      while (test_case--) {
          int x,y;
          cin >> x >> y;
          points.push_back(popup::Point2<int>({x,y}));
      }
      auto res = popup::convex_hull<int>(points.begin(), points.end());

      cout << res.size() << "\n";
      for (auto& p : res) {
          cout << p[0] << " " << p[1] << "\n";
      }

  }

  return 0;
}
