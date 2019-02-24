#include <iostream>
#include "graph.hpp"

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int num_nodes, num_edges, num_queries, starting_node;
  std::cin >> num_nodes >> num_edges >> num_queries >> starting_node;
  while (num_nodes != 0) {
    popup::Graph<int64_t> g(num_nodes);

    for (int i = 0; i < num_edges; i++) {
      int from, to, weight;
      std::cin >> from >> to >> weight;
      g.add_edge(from, to, weight);
    }
    auto res = g.bellman_ford(starting_node);

    for (int q = 0; q < num_queries; q++) {
      int to;
      std::cin >> to;
      if (!res->reachable(to)){
          std::cout << "Impossible\n";
      } else if (res->contains_cycles(to)) {
          std::cout << "-Infinity\n";
      } else {
          std::cout << res->distance_to(to).value() << "\n";
      }
    }
    std::cout << "\n";
    std::cin >> num_nodes >> num_edges >> num_queries >> starting_node;
  }
  return 0;
}
