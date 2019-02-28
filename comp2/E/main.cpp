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

  size_t num_tests;
  cin >> num_tests;

  rep(i, 0, num_tests) {
      size_t num_nodes, num_edges;
      cin >> num_nodes >> num_edges;
      popup::Graph<int> g(num_nodes);

      // in out
      vector<array<int,2>> degrees(num_nodes, {0,0});
      vector<pii> bi_edges;

      rep(e, 0, num_edges) {
          size_t from, to, one_way;
          cin >> from >> to >> one_way;
          from--;
          to--;
          if (one_way) {
              degrees[from][1] += 1;
              degrees[to][0] += 1;
              g.add_edge(from,to, 0);
          } else {
              bi_edges.push_back(make_pair(from, to));
              g.add_bi_edge(from, to, 0);
          }
      }
      for (auto& e : bi_edges) {
          auto a = e.first;
          auto b = e.second;
          if (degrees[a][0] < degrees[a][1]) {
              degrees[a][0] += 1;
              degrees[b][1] += 1;
          } else if (degrees[a][0] > degrees[a][1]) {
              degrees[a][1] += 1;
              degrees[b][0] += 1;
          }
      }

      bool all_even = true;

      for (auto& e : degrees) {
          if ((e[0] + e[1]) % 2 == 1) {
              all_even = false;
              break;
          }
      }
      if (all_even) {
          size_t num_visited =0;
          g.dfs(0, [&](const size_t node){
                       num_visited++;
                       cerr << "Node: " << node << endl;
                   });
          if (num_visited == num_nodes) {
              cout << "possible\n";
          } else {
              cout << "impossible\n";
          }
      } else {
          cout << "impossible\n";
      }
  }


}
