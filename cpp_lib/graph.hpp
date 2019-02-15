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

namespace popup {

  template <class T>

  class Edge {
    size_t from_ = 0;
    size_t to_ = 0 ;
    T weight_ = 0;

  public:
    Edge() {
    }
    Edge(size_t from, size_t to, T weight) {
      from_ = from;
      to_ = to;
      weight_ = weight;
    }
    size_t from() const {
      return from_;
    }

    size_t to() const {
      return to_;
    }

    T weight() const {
      return weight_;
    }
  };

  template <class T>
  class Graph {
  protected:
    size_t size_;
    size_t capacity_;
    std::vector<std::vector<Edge<T>>> list_;
    bool graph_modified_ = false;
    //
    std::unordered_map<
      size_t,
      std::pair<std::shared_ptr<std::vector<T>>,
                std::shared_ptr<std::vector<size_t>>>
      > cache_;
  public:
    Graph(size_t capacity) {
      capacity_ = capacity;
      size_ = 0;
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
      std::shared_ptr<std::vector<T>> distances;
      std::shared_ptr<std::vector<size_t>> came_from;
      auto cached = cache_.find(from);
      bool cache_exists = cache_.find(from) != cache_.end();

      if (!graph_modified_ && cache_exists) {

        distances = cached->second.first;
        came_from = cached->second.second;
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

        (*distances)[from] = 0;
        // Main bellman-ford part
        for (int i = 0; i < capacity_ - 1; i++) {
          for (int node = 0; node < capacity_; node++) {
            for (const auto& edge : list_[node]) {
              if ((*distances)[edge.from()] == std::numeric_limits<T>::max()) {
                continue;
              }
              T trav_cost = (*distances)[edge.from()] + edge.weight();
              //std::cerr << trav_cost << std::endl;
              if (trav_cost < (*distances)[edge.to()]) {
                (*came_from)[edge.to()] = edge.from();
                (*distances)[edge.to()] = trav_cost;
              }
            }
          }
        }

        // Identify nodes part of infinite cycles byt setting
        for (int node = 0; node < capacity_; node++) {
          for (const auto& edge : list_[node]) {
            T trav_cost = (*distances)[edge.from()] + edge.weight();
            if ((*distances)[edge.from()] == std::numeric_limits<T>::max()) {
              trav_cost = std::numeric_limits<T>::max();
            }
            if (trav_cost < (*distances)[edge.to()]) {
              (*came_from)[edge.to()] = std::numeric_limits<size_t>::max() - 1;
              (*came_from)[edge.from()] = std::numeric_limits<size_t>::max() -1;
            }
          }
        }

        cache_.insert(
          std::make_pair(
            from,
            std::make_pair(distances, came_from))
        );
      }

      if ((*came_from)[to] == std::numeric_limits<size_t>::max()) {
        return true;
      } else {
        T final_weight = (*distances)[to];

        std::vector<size_t> shortest_path;
        size_t current = to;
        if ((*came_from)[from] == std::numeric_limits<size_t>::max() - 1) {
          return false;
        }

        while (current != from) {
          if ((*came_from)[current] == std::numeric_limits<size_t>::max() - 1) {
            return false;
          }
          //shortest_path.push_back(came_from[current]);
          current = (*came_from)[current];
        }

        std::reverse(shortest_path.begin(), shortest_path.end());
        return std::make_pair(shortest_path, final_weight);
      }
    }

    void all_pairs_shortest_paths();

  };

} // namespace popup
