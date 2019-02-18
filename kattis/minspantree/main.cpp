#include <iostream>
#include "graph.hpp"

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int num_nodes, num_edges;
  std::cin >> num_nodes >> num_edges;
  while (num_nodes != 0) {
    popup::Graph<int64_t> g(num_nodes);

    for (int i = 0; i < num_edges; i++) {
      int from, to, weight;
      std::cin >> from >> to >> weight;
      if (from > to) {
          std::swap(from, to);
      }
      g.add_edge(from, to, weight);
    }

    auto res = g.kruskal();
    if (res) {
        std::cout << res->first << "\n";
        std::sort(res->second.begin(), res->second.end(),
                  [](const popup::Edge<int64_t>& a, const popup::Edge<int64_t>& b){
                      if (a.from() == b.from()) {
                          return a.to() < b.to();
                      } else {
                          return a.from() < b.from();
                      }
                  });
        for (auto& edge : res->second) {
            std::cout << edge.from() << " " << edge.to() << "\n";
        }
    } else {
        std::cout << "Impossible\n";
    }
    std::cin >> num_nodes >> num_edges;
  }
  return 0;
}
