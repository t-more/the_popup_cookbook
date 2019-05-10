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

	vector<Point2<ll>> points_y;
	vector<Point2<ll>> points_x;
	ll n;
	cin >> n;
	while(n--) {
		ll x, y;
		cin >> x >> y;
		points_x.push_back({{x, y}});
		points_y.push_back({{x, y}});
	}

	auto cmpx  = [](const Point2<ll>& a, const Point2<ll>& b) {
		return a[0] < b[0] || (a[0] == b[0] && a[1] < b[1]);
	};
	auto cmpy  = [](const Point2<ll>& a, const Point2<ll>& b) {
		return a[1] < b[1] || (a[1] == b[1] && a[0] < b[0]);
	};
	
	sort(points_x.begin(), points_x.end(), cmpx);
	sort(points_y.begin(), points_y.end(), cmpy);
	
	ll max_stan = (ll)1e10;
	for (auto& px : points_x) {
		auto vline = lower_bound(points_y.begin(), points_y.end(), px);
		while(vline != points_y.end() && (*vline)[0] == px[0]) {
			
		}

	}

	return 0;
}
