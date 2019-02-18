#include <iostream>
#include <functional>
#include <optional>
#include <vector>
#include <cassert>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>
#include <variant>
#include <unordered_map>
#include <memory>
#include "union_find.hpp"

namespace popup {

    template <class T>

    class Edge {
        size_t from_ = 0;
        size_t to_ = 0;
        T weight_ = 0;

    public:
        Edge() {
        }
        Edge(size_t from, size_t to, T weight) {
            from_ = from;
            to_ = to;
            weight_ = weight;
        }
        inline size_t from() const {
            return from_;
        }

        inline size_t to() const {
            return to_;
        }

        inline T weight() const {
            return weight_;
        }
        inline bool operator<(Edge<T> o) {
            return weight_ < o.weight_;
        }
  };

  template <class T>
  class Graph {
  protected:
      size_t size_ = 0;
      size_t capacity_;
      size_t num_edges_ = 0;

    std::vector<std::vector<Edge<T>>> list_;
    bool graph_modified_ = false;
    //
    std::unordered_map<
      size_t,
      std::tuple<
            std::shared_ptr<std::vector<T>>,
            std::shared_ptr<std::vector<size_t>>,
            std::shared_ptr<std::vector<bool>>
      >> cache_;
  public:
    Graph(size_t capacity) {
      capacity_ = capacity;
      list_ = std::vector<std::vector<Edge<T>>>(
        Graph<T>::capacity_,
        std::vector<Edge<T>>()
      );
    }

    std::optional<T> get_weight(size_t from, size_t to) {
      size_t a = to;
      auto iter = std::find_if(list_[from].begin(), list_[from].end(),
                               [&](const Edge<T> e){
                                 return e.to() == a;
                               });
      if (iter == std::end(list_[from])) {
        return std::nullopt;
      } else {
        return std::make_optional(iter->weight());
      }
    };

    void traverse_neighbors(size_t from, std::function<void(size_t, const T&)> f) {
      for(auto e : list_[from]) {
        f(e.to(), e.weight());
      }
    };

    void traverse_edges(std::function<void(Edge<T>&)> f) {
      for(auto &outer : list_) {
        for(auto &inner : outer) {
          f(inner);
        }
      }
    };
    void add_bi_edge(size_t from, size_t to, T weight) {
      add_edge(from, to, weight);
      add_edge(to, from, weight);
    };

    bool add_edge(size_t from, size_t to, T weight) {
      graph_modified_ = true;
      list_[from].emplace_back(Edge<T>(from, to, weight));
      num_edges_++;
      return true;
    };

    size_t get_size() {
      return size_;
    }
    size_t get_capacity() {
      return capacity_;
    }

    void set_weight(size_t from, size_t to, T weight) {
      graph_modified_ = true;
      auto opt_weight = get_weight(from, to);
      if (opt_weight) {
        opt_weight = weight;
      } else {
        assert(false); // opt_weight should always have a value.
      }
    };


    // This part specifies generic algorithms on graphs
    // Bfs function will terminate when f returns false
    void bfs(size_t start, std::function<bool(size_t)> f);

    void dfs(size_t start, std::function<bool(size_t)> f);

    // Dijkstra
    std::optional<std::pair<std::vector<size_t>, T>> dijkstra(size_t from, size_t to){
      const auto cmp = [](const std::pair<size_t, T>& a, const std::pair<size_t, T>& b) {
                         return a.second > b.second;
                       };
      std::priority_queue<std::pair<size_t, T>, std::vector<std::pair<size_t, T>>, decltype(cmp)> queue(cmp);
      std::vector<T> distances(capacity_, std::numeric_limits<T>::max());
      // size_t::max() is the value representing that a node never has been viewed
      std::vector<size_t> came_from(capacity_, std::numeric_limits<size_t>::max());
      std::vector<bool> visited(capacity_, 0);

      distances[from] = 0;
      queue.emplace(std::make_pair(from,0));

      while (!queue.empty()) {
        auto e = queue.top();
        queue.pop();
        auto current_node = e.first;

        if (visited[current_node]) {
          continue;
        }
        visited[current_node] = true;
        auto cost = distances[current_node];

        if (current_node == to) {
          break;
        }

        for (const auto& edge : list_[current_node]) {
          auto node = edge.to();
          const auto weight = edge.weight();
          auto node_dist = distances[node];
          auto alt_dist = weight + cost;
          if (node_dist > alt_dist) {
            distances[node] = alt_dist;
            came_from[node] = current_node;
            queue.emplace(std::make_pair(node, alt_dist));
          }
        }
      }

      if (!visited[to])  {
        return std::nullopt;
      } else {
        std::vector<size_t> shortest_path;
        size_t current = to;

        while (current != from) {
          shortest_path.push_back(came_from[current]);
          current = came_from[current];
        }
        std::reverse(shortest_path.begin(), shortest_path.end());
        return make_optional(std::make_pair(shortest_path, distances[to]));
      }
    };

    // Bellman ford returns a variant of either the result or a bool. If the
    // boolean is set to true if there simply was no path. It is false if a
    // negative cycle was detected.  If the pair is returned it contains the
    // shortest path as a vector and the weight of going there.
    std::variant<bool, std::pair<std::vector<size_t>, T>>
    bellman_ford(
      size_t from,
      size_t to
    ) {
      // Asking for a element outside the graph
      if(to >= capacity_)
          return true;

      std::shared_ptr<std::vector<T>> distances;
      std::shared_ptr<std::vector<size_t>> came_from;
      std::shared_ptr<std::vector<bool>> in_inf;
      auto cached = cache_.find(from);
      bool cache_exists = cache_.find(from) != cache_.end();
      if (!graph_modified_ && cache_exists) {
        distances = std::get<0>(cached->second);
        came_from = std::get<1>(cached->second);
        in_inf = std::get<2>(cached->second);
      } else {
        if (graph_modified_) {
          cache_.clear();
          graph_modified_ = false;
        }

        distances = std::make_shared<std::vector<T>>(
          capacity_,
          std::numeric_limits<T>::max()
        );
        came_from = std::make_shared<std::vector<size_t>>(
          capacity_,
          std::numeric_limits<size_t>::max()
        );
        in_inf = std::make_shared<std::vector<bool>>(
          capacity_,
          false
        );

        (*distances)[from] = 0;
        (*came_from)[from] = from;
        // Main bellman-ford part
        for (int i = 0; i <= (int)capacity_ + 2000; i++) {
          for (int node = 0; node < (int)capacity_; node++) {
            for (const auto& edge : list_[node]) {
              if ((*distances)[edge.from()] == std::numeric_limits<T>::max()) {
                continue;
              }
              T trav_cost = (*distances)[edge.from()] + edge.weight();
              if (trav_cost < (*distances)[edge.to()]) {
                (*came_from)[edge.to()] = edge.from();
                (*distances)[edge.to()] = trav_cost;
              }
            }
          }
        }

        // Identify nodes part of infinite cycles byt setting


        for (int node = 0; node < (int)capacity_; node++) {
            for (const auto& edge : list_[node]) {
                if ((*distances)[edge.from()] == std::numeric_limits<T>::max()) {
                    continue;
                }
                T trav_cost = (*distances)[edge.from()] + edge.weight();
                if (trav_cost < (*distances)[edge.to()]) {
                    (*in_inf)[edge.from()] = true;
                    (*in_inf)[edge.to()] = true;
                }
            }
        }


        cache_.insert(
          std::make_pair(
            from,
            std::make_tuple(distances, came_from, in_inf))
        );
      }

      if ((*came_from)[to] == std::numeric_limits<size_t>::max()) {
        return true;
      } else {
        T final_weight = (*distances)[to];

        std::vector<size_t> shortest_path;
        size_t current = to;

        if ((*in_inf)[from]) {
          return false;
        }
        //        std::cerr << "STARTING AT CURRENT: " << current << "\n";
        //        std::cerr << "ADS: " << (*came_from)[from] << std::endl;
        if (current == from && (*came_from)[from] != from) {
            //            std::cerr << "HERE\n";
            return false;
        }
        while (current != from) {
            //            std::cerr << "HERE\n";
          if ((*in_inf)[current]) {
            return false;
          }
          //          std::cerr << current <<"-";
          shortest_path.push_back((*came_from)[current]);
          current = (*came_from)[current];
        }

        std::reverse(shortest_path.begin(), shortest_path.end());
        return std::make_pair(shortest_path, final_weight);
      }
    }


      std::vector<std::vector<T>> all_pairs_shortest_paths() {

          std::vector<std::vector<T>> result(capacity_, std::vector<T>(capacity_, std::numeric_limits<T>::max()));

          // Assing initial edge costs
          for (auto& inner : list_) {
              for (auto& edge : inner) {
                  result[edge.from()][edge.to()] = std::min(result[edge.from()][edge.to()], edge.weight());
              }
          }

          // Any non assigned self loops are kept
          for (int d = 0; d < capacity_; d++) {
              if (result[d][d] > 0) {
                  result[d][d] = 0;
              }
          }

          // Main meat of the floyd warshall
          for (int k = 0; k < capacity_; k++) {
              for (int i = 0; i < capacity_; i++) {
                  for (int j = 0; j < capacity_; j++) {
                      if (result[i][k] != std::numeric_limits<T>::max()
                          && result[k][j] != std::numeric_limits<T>::max())
                      {
                          result[i][j]  = std::min(result[i][j], result[i][k] + result[k][j]);
                      }
                  }
              }
          }
          return result;
      }

      std::optional<std::pair<T,std::vector<Edge<T>>>> kruskal() {
          std::vector<Edge<T>> all_edges(num_edges_);
          int total_edge_count = 0;
          for (auto& edges : list_) {
              for (auto& edge : edges) {
                  all_edges[total_edge_count++] = edge;
              }
          }
          std::sort(all_edges.begin(), all_edges.end());
          popup::UnionFind uf(capacity_);

          std::vector<Edge<T>> result(capacity_ - 1);

          // Dis issss kruskaaaal
          int mst_edge_count = 0;
          T cost = 0;
          for (auto& edge : all_edges) {
              if (uf.find(edge.from()) != uf.find(edge.to())) {

                  cost += edge.weight();
                  uf.make_union(edge.from(), edge.to());
                  result[mst_edge_count++] = edge;
                  if (mst_edge_count >= capacity_ -1) {
                      return std::make_pair(cost, result);
                  }
              }
          }
          return std::nullopt;
      }

  };

} // namespace popup
