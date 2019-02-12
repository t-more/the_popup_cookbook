#include <functional>
#include <optional>
#include <vector>
#include <cassert>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>

namespace popup {

  template <class T>
  class Edge {
    size_t from_;
    size_t to_;
    T weight_;
  public:
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

    T& weight() {
      return weight_;
    }
  };

  template <class T>
  class Graph {
  protected:
    size_t size_;
    size_t capacity_;
  public:
    size_t get_size() {
      return size_;
    }
    size_t get_capacity() {
      return capacity_;
    }

    virtual std::optional<T> get_weight(size_t from, size_t to)=0;
    virtual void set_weight(size_t from, size_t to, T weight) {
      auto opt_weight = get_weight(from, to);
      if (opt_weight) {
        opt_weight = weight;
      } else {
        assert(false); // opt_weight should always have a value.
      }
    };

    virtual void traverse_neighbors(size_t node, std::function<void(size_t, const T&)> f)=0;
    virtual void traverse_edges(std::function<void(Edge<T>&)> f)=0;
    virtual bool add_edge(size_t from, size_t to, T weight)=0;
    //    virtual void get_edge(size_t from, size_t to)=0;
    virtual void add_bi_edge(size_t from, size_t to, T weight) {
      add_edge(from, to, weight);
      add_edge(to, from, weight);
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

        traverse_neighbors(current_node,
                           [&](size_t node, const T& weight){
                             auto node_dist = distances[node];
                             auto alt_dist = weight + cost;
                             if (node_dist > alt_dist) {
                               distances[node] = alt_dist;
                               came_from[node] = current_node;
                               queue.emplace(std::make_pair(node, alt_dist));
                             }
                           });
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

    // Bellman ford
    void bellman_ford(size_t from, size_t to);

    void all_pairs_shortest_paths();

  };

  template <class T>
  class ListGraph : public Graph<T> {
    std::vector<std::vector<Edge<T>>> list_;
  public:
    ListGraph(size_t capacity) {
      Graph<T>::capacity_ = capacity;
      Graph<T>::size_ = 0;
      list_ = std::vector<std::vector<Edge<T>>>(Graph<T>::capacity_, std::vector<Edge<T>>());
    }

    virtual std::optional<T> get_weight(size_t from, size_t to) {
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

    virtual void traverse_neighbors(size_t from, std::function<void(size_t, const T&)> f) {
      for(auto e : list_[from]) {
        f(e.to(), e.weight());
      }
    };

    virtual void traverse_edges(std::function<void(Edge<T>&)> f) {
      for(auto &outer : list_) {
        for(auto &inner : outer) {
          f(inner);
        }
      }
    };

    virtual bool add_edge(size_t from, size_t to, T weight) {
        list_[from].emplace_back(Edge<T>(from, to, weight));
        return true;
    };
  };
} // namespace popup
