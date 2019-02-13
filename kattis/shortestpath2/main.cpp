#include <iostream>
#include "time_table_graph.hpp"
#include <cassert>

int main() {
  int num_nodes, num_edges, num_queries, starting_node;
  std::cin >> num_nodes >> num_edges >> num_queries >> starting_node;
  while (num_nodes != 0) {
    popup::Graph g(num_nodes);

    for (int i = 0; i < num_edges; i++) {
      int from, to, start, period, weight;
      std::cin >> from >> to >>  start >> period >> weight;
      g.add_edge(from, to, start, period, weight);
    }

    for (int q = 0; q < num_queries; q++) {
      int to;
      std::cin >> to;

      auto res = g.dijkstra(starting_node, to);
      if (!res) {
        std::cout << "Impossible\n";
      } else {
        std::cout << res->second << "\n";
      }
    }
    std::cout << "\n";
    std::cin >> num_nodes >> num_edges >> num_queries >> starting_node;
  }
  return 0;
}
