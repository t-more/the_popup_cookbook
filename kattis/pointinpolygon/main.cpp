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

    int n;
    while(cin >> n && n != 0) {
        vector<popup::Vec2<double>> polygon(n);
        for (auto &v : polygon) {
            cin >> v[0] >> v[1];
        }

        int tests;
        cin >> tests;
        while(tests--) {
            popup::Vec2<double> v;
            cin >> v[0] >> v[1];
            auto res = popup::point_in_polygon<double>(
                v, 
                polygon.begin(), 
                polygon.end()
            );

            switch(res) {
                case popup::PointLocation::Outside:
                    cout << "out\n";
                    break;
                case popup::PointLocation::Inside:
                    cout << "in\n";
                    break;
                case popup::PointLocation::Border:
                    cout << "on\n";
                    break;
            }

        }
    }

    return 0;
}
