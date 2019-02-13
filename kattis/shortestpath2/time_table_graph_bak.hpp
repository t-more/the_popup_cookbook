#include <functional>
#include <optional>
#include <vector>
#include <cassert>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <tuple>



namespace std {
  template<typename T1, typename T2>
  struct hash<std::pair<T1, T2>> {
    // Code idea stolen shamelessy from https://www.techiedelight.com/use-std-pair-key-std-unordered_map-cpp/
    std::size_t operator()(std::pair<T1, T2> const &p) const {
      return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
    }
  };
}

namespace popup {

  template<typename T>
  inline T mod(T n, T m) {
    const int res = n % m;
    return res >= 0 ? res : res + m;
  }

  class Edge {
    size_t from_;
    size_t to_;
    int weight_;
    size_t starting_time_;
    size_t period_;
  public:
    Edge(size_t from, size_t to, size_t starting_time, size_t period, int weight) {
      from_ = from;
      to_ = to;
      weight_ = weight;
      starting_time_ = starting_time;
      period_ = period;
    }
    size_t from() const {
      return from_;
    }

    size_t to() const {
      return to_;
    }

    size_t starting_time() const {
      return starting_time_;
    }

    size_t period() const {
      return period_;
    }

    int& weight() {
      return weight_;
    }
  };

  class Graph {
    size_t size_;
    size_t capacity_;
    std::vector<std::vector<Edge>> list_;
  public:
    Graph(size_t size) {
      size_ = size;
      capacity_ = size;
      list_.resize(size);
    }
    size_t get_size() {
      return size_;
    }
    size_t get_capacity() {
      return capacity_;
    }

    bool add_edge(size_t from, size_t to, int start, int period, int weight) {
      list_[from].emplace_back(Edge(from, to, start, period, weight));
      return true;
    };

    // Dijkstra
    std::optional<std::pair<std::vector<size_t>, int>> dijkstra(size_t from, size_t to){
      const auto cmp = [](const std::pair<size_t, int>& a, const std::pair<size_t, int>& b) {
                         return a.second > b.second;
                       };
      std::priority_queue<
        std::pair<size_t, int>,
        std::vector<std::pair<size_t, int>>,
        decltype(cmp)> queue(cmp);

      std::unordered_map<std::pair<size_t, size_t>, std::pair<size_t, size_t>> distances;

      std::unordered_map<std::pair<size_t, size_t>, std::pair<size_t, size_t>> came_from;

      std::unordered_set<std::pair<size_t, size_t>> visited;

      queue.emplace(std::make_pair(from, 0));

      size_t time_done = std::numeric_limits<size_t>::max();
      while (!queue.empty()) {
        auto e = queue.top();
        queue.pop();
        auto current_node = e.first;
        auto current_time = e.second;

        if (visited.find(e) != visited.end()) {
          continue;
        }

        visited.emplace(e);
        auto cost = distances.find(e)->second;
        if (current_node == to) {
          time_done = e.second;
          break;
        }

        for (auto& edge : list_[current_node]) {
          auto node_dist_iter = distances.find(std::make_pair(edge.to(), current_time));
          size_t node_dist = node_dist_iter =
            distances.end() ? std::numeric_limits<size_t>::max() : node_dist_iter->second;

          //in progress
          size_t t_to_open = (edge.period() - 1) - mod(t - edge.starting_time(), edge.period());
          size_t time_to_trav = current_time + edge.weight() + t_to_open;
          queue.emplace(
            std::make_pair(edge.from(), time_to_trav)
          );
          came_from.insert(
            std::make_pair(edge.from(), time_to_trav),
            std::make_pair(current_node, current_time)
          );
        }
      }


      if (time_done == std::numeric_limits<size_t>::max())  {
        return std::nullopt;
      } else {
        std::vector<size_t> shortest_path;
        auto current = make_pair(to, time_done);

        auto start = make_pair(from, 0);
        while (current != start) {
          auto prev = *came_from.find(current);
          shortest_path.push_back(prev);
          current = prev;
        }
        std::reverse(shortest_path.begin(), shortest_path.end());
        return make_optional(std::make_pair(shortest_path, time_done));
      }
    };

  };
} // namespace popup
