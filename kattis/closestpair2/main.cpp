#include <bits/stdc++.h>
#include "closest_pair.hpp"
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

using shittype = double;

int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);

  size_t num_points;

  vector<Point2<shittype>> points;
  //  cout << setprecision(2);
  cout << fixed;
  while (cin >> num_points && num_points != 0) {
      points.clear();
      for (size_t i = 0; i < num_points; i++) {
          double a, b;
          cin >> a >> b;
          points.push_back(
              Point2<shittype>({(shittype)(a * 1)
                               , (shittype)(b * 1)})
          );
      }

      auto [p1,p2] = closest_pair<shittype>(points.begin(), points.end());
      cout << ((double)p1[0] / 1) << " " << ((double)p1[1] / 1)
           << " " << ((double)p2[0] / 1) << " " << ((double)p2[1] / 1)
           << "\n";
  }


}
