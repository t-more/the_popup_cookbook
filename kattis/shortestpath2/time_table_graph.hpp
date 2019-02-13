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

namespace popup {

  template<typename T>
  inline T mod(T n, T m) {
      const int res = n % m;
      return res >= 0 ? res : res + m;
  }

  class Edge {
    size_t from_;
    size_t to_;
    int64_t weight_;
    size_t starting_time_;
    size_t period_;
  public:
    Edge(size_t from, size_t to, size_t starting_time, size_t period, int64_t weight) {
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

    int64_t& weight() {
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

    bool add_edge(size_t from, size_t to, int64_t start, int64_t period, int64_t weight) {
      list_[from].emplace_back(Edge(from, to, start, period, weight));
      return true;
    };

    // Dijkstra
    std::optional<std::pair<std::vector<size_t>, int64_t>> dijkstra(size_t from, size_t to){
      const auto cmp = [](const std::pair<size_t, int64_t>& a, const std::pair<size_t, int64_t>& b) {
                         return a.second > b.second;
                       };

      std::priority_queue<std::pair<size_t, int64_t>,
                          std::vector<std::pair<size_t, int64_t>>,
                          decltype(cmp)> queue(cmp);
      std::vector<int64_t> distances(capacity_, std::numeric_limits<int64_t>::max());
      // size_t::max() is the value representing that a node never has been viewed
      std::vector<size_t> came_from(capacity_, std::numeric_limits<size_t>::max());
      std::vector<bool> visited(capacity_, 0);

      distances[from] = 0;
      queue.emplace(std::make_pair(from,0));

      while (!queue.empty()) {
        auto e = queue.top();
        queue.pop();
        auto current_node = e.first;
        auto current_time = e.second;

        if (visited[current_node]) {
          continue;
        }
        visited[current_node] = true;
        if (current_node == to) {
          break;
        }

        for (auto& edge : list_[current_node]) {
          auto node = edge.to();
          //in progress
          size_t t_to_open;

          if (current_time <= edge.starting_time()) {
            t_to_open = edge.starting_time() - current_time;
          } else if (edge.period() == 0) {
            continue;
          } else {
            t_to_open = (edge.period() - ((current_time - edge.starting_time()) % edge.period())) % edge.period();
          }

          size_t time_to_trav = current_time + edge.weight() + t_to_open;
          // std::cerr << "dist: " << distances[node] << " ttt: " << time_to_trav << "\n";
          // std::cerr << "from: " << current_node << " to: " << node << "\n";
          // std::cerr << "CC: " << current_time << " tto:" << t_to_open << "\n";

          if (distances[node] > time_to_trav) {
            distances[node] = time_to_trav;
            queue.push(
              std::make_pair(edge.to(), time_to_trav)
            );
            came_from[node] = current_node;
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
        return std::make_optional(std::make_pair(shortest_path, distances[to]));
      }
    };

  };
} // namespace popup
