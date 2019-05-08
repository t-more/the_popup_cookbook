#include <bits/stdc++.h>
#include "polygons.hpp"

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
  vector<popup::Point2<double>> points;
  size_t num_points;
  while (cin >> num_points && num_points != 0) {
      points.clear();
      while (num_points--) {
          double x,y;
          cin >> x >> y;
          popup::Point2<double> t({x,y});
          points.push_back(t);
      }

      auto [order, area] = popup::polygon_order_area<double>(
          points.begin(),
          points.end()
      );
      cout << std::setprecision(1) << std::fixed << order << " " << area << endl;
  }
  return 0;
}
