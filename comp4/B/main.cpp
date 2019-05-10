#include <bits/stdc++.h>
#include "point.hpp"
using namespace std;
using namespace popup;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin, x.end()
#define sx(x) (int)(x).size()
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;

int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);


  double x1, y1, x2,y2;

  cin >> x1 >> y1 >> x2 >> y2;
  cout << setprecision(3);
  cout << fixed;
  Point2<double> gopher({x1,y1});
  Point2<double> dog({x2,y2});
  double hx, hy;
  while (cin >> hx >> hy) {
      Point2<double> hole({hx,hy});
      if (2 * gopher.distance_to(hole) - 1e-9 <= dog.distance_to(hole)) {
          cout << "The gopher can escape through the hole at (" << hx
               << "," << hy <<  ").\n";
              exit(0);
      }
  }
  cout << "The gopher cannot escape.\n";

}
