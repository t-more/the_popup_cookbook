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

int64_t next_int64() {
  char c = getchar_unlocked();
  char prefix_char = 0;
  while ('0' > c || '9' < c) {
    prefix_char = c;
    c = getchar_unlocked();
  }
  int x = 0;
  while ('0' <= c && c <= '9') {
    x = 10 * x + c - '0';
    c = getchar_unlocked();
  }
  return (((prefix_char != '-') << 1) - 1) * x;
}

double next_double() {
  char c = getchar_unlocked();
  char prefix_char = 0;
  while ('0' > c || '9' < c) {
    prefix_char = c;
    c = getchar_unlocked();
  }
  int64_t x = 0;
  while ('0' <= c && c <= '9') {
    x = 10 * x + c - '0';
    c = getchar_unlocked();
  }
  if (c == '.') {
      c = getchar_unlocked();
      int following = 0;
      while ('0' <= c && c <= '9') {
          x = 10 * x + c - 48;
          c = getchar_unlocked();
          following++;
      }
      if (following == 2) {
          return (double)((((prefix_char != '-') << 1) - 1) * x) / (double)100;
      } else {
          return (double)((((prefix_char != '-') << 1) - 1) * x) / (double)10;
      }
  }
  return (double)((((prefix_char != '-') << 1) - 1) * x);
}

uint32_t next_word32() {
  char c = getchar_unlocked();
  while ('0' > c || '9' < c) {
    c = getchar_unlocked();
  }
  int x = 0;
  while ('0' <= c && c <= '9') {
    x = 10 * x + c - 48;
    c = getchar_unlocked();
  }
  return x;
}

int main() {

  vector<Point2<shittype>> points;
  //  cout << setprecision(2);
  cout << fixed;
  for (auto num_points = next_word32(); num_points != 0; num_points = next_word32()) {
      points.clear();
      for (size_t i = 0; i < num_points; i++) {
          double a = next_double(), b = next_double();
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
