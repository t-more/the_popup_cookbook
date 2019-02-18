#include <iostream>
#include <set>
#include <limits>
#include "graph.hpp"

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int num_nodes, num_edges, num_queries, starting_node;
  std::cin >> num_nodes >> num_edges >> num_queries;
  while (num_nodes != 0) {
    popup::Graph<int64_t> g(num_nodes);

    for (int i = 0; i < num_edges; i++) {
      int from, to, weight;
      std::cin >> from >> to >> weight;
      g.add_edge(from, to, weight);
    }
    auto res = g.all_pairs_shortest_paths();
    std::set<size_t> negative_nodes;

    for (int d = 0; d < res.size(); d++) {
        if (res[d][d] < 0) {
            negative_nodes.insert(d);
        }
    }

    for (int q = 0; q < num_queries; q++) {
        int from, to;
        std::cin >> from >> to;

        bool found_inf = false;
        for (auto e : negative_nodes) {
            auto to_e = res[from][e];
            auto from_e = res[e][to];
            if (to_e != std::numeric_limits<int64_t>::max() &&
                from_e != std::numeric_limits<int64_t>::max()) {
                std::cout << "-Infinity\n";
                found_inf = true;
                break;
            }
        }
        if (found_inf) {
            continue;
        }
        auto dist = res[from][to];
        if (dist == std::numeric_limits<int64_t>::max()) {
            std::cout << "Impossible\n";
        } else {
            std::cout << dist << "\n";
        }
    }
    std::cout << "\n";
    std::cin >> num_nodes >> num_edges >> num_queries;
  }
  return 0;
}
