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

  size_t num_nodes, num_edges, known_nodes;
  cin >> num_nodes >> num_edges >> known_nodes;
  popup::Graph<char> graph(num_nodes);
  popup::Graph<char> rev_graph(num_nodes);

  vector<bool> known(num_nodes);
  stack<size_t> to_visit;

  rep (e, 0, num_edges) {
      size_t from, to;
      cin >> from >> to;
      from--;
      to--;
      graph.add_edge(from, to, 0);
      rev_graph.add_edge(to, from, 0);
  }

  rep (i, 0, known_nodes) {
      size_t n;
      cin >> n;
      n--;
      known[n] = true;
      to_visit.push(n);

      if (!rev_graph.list_[n].empty()) {
          unordered_set<size_t> final_nodes;
          for (auto& e : rev_graph.list_[n]) {
              size_t final_node = -1;
              rev_graph.dfs(e.to(), [&](const size_t node){
                                        final_node = node;
                                    });
              final_nodes.insert(final_node);
          }
          if (final_nodes.size() == 1) {
              size_t other_node = *final_nodes.begin();
              if (!known[other_node]) {
                  known[other_node] = true;
                  to_visit.push(other_node);
              }
          }
      }
  }

  while (!to_visit.empty()) {
      int node = to_visit.top();
      to_visit.pop();
      graph.dfs(node, [&](const size_t node){
                          if (!known[node]) {
                              known[node] = true;
                              to_visit.push(node);

                              if (!rev_graph.list_[node].empty()) {
                                  unordered_set<size_t> final_nodes;
                                  for (auto& e : rev_graph.list_[node]) {
                                      size_t final_node = -1;
                                      rev_graph.dfs(e.to(), [&](const size_t node){
                                                                final_node = node;
                                                            });
                                      final_nodes.insert(final_node);
                                  }
                                  if (final_nodes.size() == 1) {
                                      size_t other_node = *final_nodes.begin();
                                      if (!known[other_node]) {
                                          known[other_node] = true;
                                          to_visit.push(other_node);
                                      }
                                  }
                              }
                          }
                      });
  }

  rep(node, 0, num_nodes) {
      if (known[node]) {
          cout << (node + 1) << " ";
      }
  }
  cout << endl;


}
