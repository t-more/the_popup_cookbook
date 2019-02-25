#include <bits/stdc++.h>
#include "flow_graph.hpp"
using namespace std;


int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);
  uint64_t num_nodes, num_edges, source, sink;
  cin >> num_nodes >> num_edges >> source >> sink;
  popup::FlowGraph<uint64_t, uint64_t> graph(num_nodes);
  uint64_t from, to, capacity, cost;

  for (int n = 0; n < (int)num_edges; n++) {
        cin >> from >> to >> capacity >> cost;
        graph.add_edge(from, to, capacity, cost);
  }
  auto res = graph.min_cost_max_flow(source, sink);
  cout << res.first << " " << res.second;

  return 0;
}
