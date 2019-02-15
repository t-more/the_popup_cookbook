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

    for (int q = 0; q < num_queries; q++) {
      int to;
      std::cin >> to;
      auto res = g.bellman_ford(starting_node, to);

      if (std::holds_alternative<bool>(res)) {
        if (std::get<bool>(res)) {
          std::cout << "Impossible\n";
        } else {
          std::cout << "-Infinity\n";
        }
      } else {
        auto elem = std::get<std::pair<std::vector<size_t>, int64_t>>(res);
        std::cout << elem.second << "\n";
      }
    }
    std::cout << "\n";
    std::cin >> num_nodes >> num_edges >> num_queries >> starting_node;
  }
  return 0;
}
