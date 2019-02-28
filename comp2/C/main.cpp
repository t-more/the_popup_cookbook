#include <bits/stdc++.h>
#include "flow_graph.hpp"
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin, x.end()
#define sx(x) (int)(x).size()
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;
typedef vector<bool> vb;

int main() {
	cin.sync_with_stdio(0);
	cin.tie(0);
	cin.exceptions(cin.failbit);


	int tests;
	cin >> tests;

	while(tests--) {
		int prisoners, pairs;
		cin >> prisoners >> pairs;
		int source = 0;
		int sink = 1;
		vector<vb> dang_pairs(prisoners+2, vb(prisoners+2, false));
		rep(i, 0, pairs) {
			int x, y;
			cin >> x >> y;
			x--; y--;
			dang_pairs[x][y] = true;
			//dang_pairs[y][x] = true;
		}
		
		popup::FlowGraph<int, int> graph(2*prisoners+2);
		rep(x, 2, prisoners+2) {
			rep(y, 2, prisoners+2) {
				if(!dang_pairs[x-2][y-2]) {
					graph.add_edge(x, prisoners + y, 1, 1);
				}
			}
		}
		
		rep(x, 2, prisoners+2) {
			graph.add_edge(0, x, 1, 1);
			graph.add_edge(prisoners + x, 1, 1, 1);
		}

		auto res = graph.edmond_karp(0,1);
		cout << (res/2) << endl;
	}


}
