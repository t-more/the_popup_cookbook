#include <bits/stdc++.h>

using namespace std;

double fix_zero(double d) {
     if (abs(d) < 0.01) {
         return 0;
     } else {
         return d;
     }
 }


template<class T>
struct Point {
        typedef Point P;
        T x, y;
        explicit Point(T x=0, T y=0) : x(x), y(y) {}
        bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
        bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
        P operator+(P p) const { return P(x+p.x, y+p.y); }
        P operator-(P p) const { return P(x-p.x, y-p.y); }
        P operator*(T d) const { return P(x*d, y*d); }
        P operator/(T d) const { return P(x/d, y/d); }
        T dot(P p) const { return x*p.x + y*p.y; }
        T cross(P p) const { return x*p.y - y*p.x; }
        T cross(P a, P b) const { return (a-*this).cross(b-*this); }
        T dist2() const { return x*x + y*y; }
        double dist() const { return sqrt((double)dist2()); }
        // angle to x-axis in interval [-pi, pi]
        double angle() const { return atan2(y, x); }
        P unit() const { return *this/dist(); } // makes dist()=1
        P perp() const { return P(-y, x); } // rotates +90 degrees
        P normal() const { return perp().unit(); }
        // returns point rotated 'a' radians ccw around the origin
        P rotate(double a) const {
                return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
};

template<class P>
int segmentIntersection(const P& s1, const P& e1,
                const P& s2, const P& e2, P& r1, P& r2) {
        if (e1==s1) {
                if (e2==s2) {
                        if (e1==e2) { r1 = e1; return 1; } //all equal
                        else return 0; //different point segments
                } else return segmentIntersection(s2,e2,s1,e1,r1,r2);//swap
        }
        //segment directions and separation
        P v1 = e1-s1, v2 = e2-s2, d = s2-s1;
        auto a = v1.cross(v2), a1 = v1.cross(d), a2 = v2.cross(d);
        if (a == 0) { //if parallel
                auto b1=s1.dot(v1), c1=e1.dot(v1),
                     b2=s2.dot(v1), c2=e2.dot(v1);
                if (a1 || a2 || max(b1,min(b2,c2))>min(c1,max(b2,c2)))
                        return 0;
                r1 = min(b2,c2)<b1 ? s1 : (b2<c2 ? s2 : e2);
                r2 = max(b2,c2)>c1 ? e1 : (b2>c2 ? s2 : e2);
                return 2-(r1==r2);
        }
        if (a < 0) { a = -a; a1 = -a1; a2 = -a2; }
        if (0<a1 || a<-a1 || 0<a2 || a<-a2)
                return 0;
        r1 = s1-v1*a2/a;
        return 1;
}

int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);

  size_t num_testcases;
  cin >> num_testcases;
  for (int i = 0; i < (int)num_testcases; i++) {
      Point<double> p1, p2, p3, p4;
      cin >> p1.x >> p1.y >> p2.x >> p2.y >>
          p3.x >> p3.y >> p4.x >> p4.y;
      Point<double> left, right;
      auto ans = segmentIntersection<Point<double>>(p1, p2, p3, p4, left, right);


      cout << setprecision(2) << fixed;
      switch (ans) {
      case 0:
          cout << "none\n";
          break;
      case 2:
          if (left.x > right.x) std::swap(left, right);
          if (left.x == right.x && left.y > right.y) std::swap(left, right);
          cout << fix_zero(left.x) << " " << fix_zero(left.y) << " "
               << fix_zero(right.x) << " " << fix_zero(right.y) << "\n";
          break;
      case 1:
          cout << fix_zero(left.x) << " "
               << fix_zero(left.y) << "\n";
          break;
      }
  }

  return 0;
}
