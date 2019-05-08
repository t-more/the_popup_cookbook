#include <bits/stdc++.h>
#include "line_segment.hpp"
using namespace std;
using namespace popup;

#define rep(i, a, b) for(int i = a; i < (int)(b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
#define EPS 10e-9

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

double fix_zero(double d) {
    if (abs(d) < 0.01) {
        return 0;
    } else {
        return d;
    }
}

int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);

  size_t num_testcases;
  cin >> num_testcases;
  rep(i,0, num_testcases) {
      Point<2, double> p1, p2, p3, p4;
      cin >> p1[0] >> p1[1] >> p2[0] >> p2[1] >>
          p3[0] >> p3[1] >> p4[0] >> p4[1];
      LineSegment<double> a = {p1,p2}, b = {p3,p4};
      auto ans = a.intersection(b);

      cout << setprecision(2) << fixed;
      if (ans.has_value()) {
          if (std::holds_alternative<Point2<double>>(*ans)) {
              auto point = std::get<Point2<double>>(*ans);
              cout << fix_zero(point[0])
                   << " "
                   << fix_zero(point[1])
                   << "\n";
          } else {
              auto segment = std::get<LineSegment<double>>(*ans);
              cout << fix_zero(segment.min_point()[0])
                   << " "
                   << fix_zero(segment.min_point()[1])
                   << " "
                   << fix_zero(segment.max_point()[0])
                   << " "
                   << fix_zero(segment.max_point()[1])
                   << "\n";
          }
      }else {
          cout << "none\n";
      }
  }

  return 0;
}
