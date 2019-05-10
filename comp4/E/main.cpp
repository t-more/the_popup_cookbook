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

long double dist(vector<Point2<long double>>& points, Point2<long double>& point) {
    long double s = 0;
    for (auto p : points) {
        s += p.square_distance_to(point);
    }
    return sqrt(s);
}

int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);
  cout << setprecision(15);
  cout << fixed;
  size_t num_points;
  cin >> num_points;
  vector<Point2<long double>> points;

  long double avg_x = 0, avg_y =0;
  rep(i, 0, num_points) {
      double x, y;
      cin >> x >> y;
      avg_x += x;
      avg_y += y;
      points.push_back(Point2<long double>({x,y}));
  }

  long double x, y;
  x = avg_x  / (long double)num_points;
  y = avg_y  / (long double)num_points;
  Point2<long double> point({x,y});
  long double current_val = dist(points, point);
  cerr << current_val << endl;
  long double diff = 1.0;
  int i = 0;
  while (abs(diff) > 1e-10) {
      //      cerr << "Here "  << endl;
      Point2<long double> c1({x+ 1.0,y});
      Point2<long double> c2({x,y + 1.0});
      auto c1_val = dist(points, c1);
      auto c2_val = dist(points, c2);
      auto c1_better = current_val - c1_val;
      auto c2_better = current_val - c2_val;
      Point2<long double> better = point + Point2<long double>({c1_better, c2_better}) * 0.1;
      //      cerr << "BV: " << better << endl;
      double new_val = dist(points, point);
      diff =  new_val - current_val;
      //      cerr << "cv: "<< current_val << endl;
      //      cerr << "new val: "<< new_val << endl;
      //      cerr << "Dif: " << diff << endl;
      point = better;
      current_val = new_val;
      if(i++ == 100) {
          //exit(0);
      }
  }
  cout << (double)(2 * current_val) << "\n";
}
