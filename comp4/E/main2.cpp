#include <bits/stdc++.h>
#include "point.hpp"
using namespace std;
using namespace popup;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin, x.end()
#define sx(x) (int)(x).size()
typedef __int128 ll;
typedef pair<int,int> pii;
typedef vector<int> vi;

ll p = 10000000;
int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);

  size_t num_points;
  cin >> num_points;
  vector<Point2<ll>> points;

  ll avg_x = 0, avg_y =0;
  rep(i, 0, num_points) {
      int64_t x, y;
      cin >> x >> y;
      avg_x += x * p;
      avg_y += y * p;
      points.push_back(Point2<ll>({x * p ,y * p }));
  }

  ll x, y;
  x = avg_x  / num_points;
  y = avg_y  / num_points;
  Point2<ll> mid = Point2<ll>({x,y});
  //  cout << mid << endl;
  cout << setprecision(15);
  cout << fixed;
  ll sum = 0;
  for (auto& e : points) {
      sum += mid.square_distance_to(e);
  }
  cout << 2 * sqrt((double)(sum)) / p << "\n";
}
