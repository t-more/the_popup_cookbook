#include <bits/stdc++.h>
#include "point.hpp"
#include "vec.hpp"
#include "line_segment.hpp"

using namespace std;
using namespace popup;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin, x.end()
#define sx(x) (int)(x).size()
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;


enum PointLocation {
                    Outside = 0,
                    Border,
                    Inside
};

template<typename T, typename RAItr>
pair<PointLocation, double> point_in_polygon(
    const Point2<T>& point,
    RAItr begin,
    RAItr end
) {
    const T EPS = 1e-0;
    double angle_sum = 0;
    auto prev = *begin;
    auto it = begin;
    double min_dist = numeric_limits<double>::max();
    it++;
    for (; it != end; it++) {
        Vec2<T> v1 = Vec2<T>(*it - point).normalized();
        Vec2<T> v2 = Vec2<T>(prev - point).normalized();
        double cross_angle = cross(v1, v2);
        double dot_angle = v1.dot(v2);
        angle_sum += std::atan2(cross_angle, dot_angle);
        if (LineSegment<T>(prev, *it).contains_point(point)) {
            return make_pair(PointLocation::Border, 0);
        }
        min_dist = min(LineSegment<T>(prev, *it).distance_to(point)
                       , min_dist);

        prev = *it;
    }
    min_dist = min(LineSegment<T>(*begin, *(end -1)).distance_to(point)
                   , min_dist);

    Vec2<T> v1 = Vec2<T>(*begin - point).normalized();
    Vec2<T> v2 = Vec2<T>(prev - point).normalized();
    double cross_angle = cross(v1, v2);
    double dot_angle = v1.dot(v2);
    angle_sum += std::atan2(cross_angle, dot_angle);
    if (LineSegment<T>(prev, *begin).contains_point(point)) {
        return make_pair(PointLocation::Border, 0);
    }

    if (std::abs(angle_sum) < EPS) {
        return make_pair(PointLocation::Outside, min_dist);
    } else {
        return make_pair(PointLocation::Inside, min_dist);
    }
}

int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);

  cout << fixed;
  cout << setprecision(15);
  vector<Point2<double>> line_points;
  vector<Point2<double>> points;
  size_t num_lines;
  int i = 1;
  while (cin >> num_lines && num_lines != 0) {
      line_points.clear();
      points.clear();
      cout << "Case " << i++ << "\n";
      for (int i = 0; i < num_lines; i++) {
          double a, b;
          cin >> a >> b;
          line_points.push_back(Point2<double>({a,b}));
      }
      size_t num_points;
      cin >> num_points;
      for(int i = 0; i < num_points; i++) {
          double p1, p2;
          cin >> p1 >> p2;
          //          cerr << "P" << p1 << " " << p2;
          auto [type,dist] = point_in_polygon(
              Point2<double>({p1,p2}),
              line_points.begin(),
              line_points.end()
          );
          if (type == PointLocation::Outside) {
              cout << "Miss! " << dist << "\n";
          } else if (type == PointLocation::Inside) {
              cout << "Hit! " << dist << "\n";
          } else {
              cout << "Winged!\n";
          }
      }
  }


}
