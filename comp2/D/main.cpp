#include <bits/stdc++.h>
#include "./graph.hpp"
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
  cin.exceptions(cin.failbit);

  size_t num_nodes, num_edges;
  cin >> num_nodes >> num_edges;

  popup::Graph<size_t> g(num_nodes);

  rep(e, 0, num_edges) {
      size_t from, to, cost;
      cin >> from >> to >> cost;
      g.add_bi_edge(from,to,cost);
  }
  size_t num_stores;
  cin >> num_stores;
  num_stores += 1;
  vector<size_t> stores(num_stores);

  rep(s,1,num_stores) {
      size_t store;
      cin >> store;
      stores[s] = store;
  }
  stores[0] = 0;

  vector<vector<size_t>> dirs(num_stores+1, vector<size_t>(num_stores, 0));
  rep (from, 0, num_stores) {
      rep (to, from+1, num_stores) {
          auto dist = g.quick_dijkstra(stores[from], stores[to]);
          dirs[from][to] = dist;
          dirs[to][from] = dist;
      }
  }

  size_t best = -1;

  vector<size_t> path(num_stores, 0);
  iota(path.begin() , path.end(), 0);

  do {
      size_t dist = 0;
      rep(i,0,num_stores) {
          dist += dirs[path[i]][path[(i+1)% num_stores]];
      }
      best = min(dist, best);
  } while (next_permutation(path.begin()+1, path.end()));
  cout << best;

}
