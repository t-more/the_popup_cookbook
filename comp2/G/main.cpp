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

int main() {
	cin.sync_with_stdio(0);
	cin.tie(0);

	int glads;
	while(cin >> glads && glads != 0) {

		unordered_map<int,string> glad_name;
		unordered_map<int, int> glad_stats;
		int index = 0;
		
		int joining_glads = 1 << (int)(floor(log2(glads)));

		if (joining_glads == glads) {
			cout << 0 << "\n";
			continue;
		}
			
		rep(i,0,glads) {
			string name;
			int ability;
			cin >> name >> ability;
			glad_name[index] = name;
			glad_stats[index] = ability;
			index++;
		}
		
		popup::FlowGraph<int, int> g(glads + 4);
		int s = glads;
		int t = glads+1;
		int s2 = glads+2;
		int t2 = glads+3;
		g.add_edge(s2, s, joining_glads/2, 0);
		g.add_edge(t2, t, joining_glads/2, 0);
		rep(x, 0, glads) {
			g.add_edge(s, x, 1, 0);
			g.add_edge(x, t, 1, 0);
			rep(y, x+1, glads) {
				g.add_edge(x, y, 1, 
						- (1/pow(glad_stats[x]-glad_stats[y], 2)));
			}
		}

		vector<bool> matched(glads, false);
		g.min_cost_max_flow(s2, t2);    
		rep(i, 0, glads) {
			for(auto &edge : g.list_[i]) {
				if(edge->residual() == 0 && edge->to() != s &&
						edge->to() != t) {
					cerr << i << " " << edge->to() << endl;
					matched[i] = true;
					//matched[edge->to()] = true;
					break;
				}
			}
		}

		cout << (glads - joining_glads) << "\n";
		for(int i = 0; i < glads; i++) {
			if(!matched[i]) {
				cout << glad_name[i] << "\n";
			}
		}
	}
}
