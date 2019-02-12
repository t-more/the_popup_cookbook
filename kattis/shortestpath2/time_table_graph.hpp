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

  template <class T>
  class Edge {
    size_t from_;
    size_t to_;
    T weight_;
    size_t starting_time_;
    size_t period_;
  public:
    Edge(size_t from, size_t to, T weight, size_t starting_time, size_t period) {
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

    T& weight() {
      return weight_;
    }
  };

  template <class T>
  class Graph {
    size_t size_;
    size_t capacity_;
  public:
    size_t get_size() {
      return size_;
    }
    size_t get_capacity() {
      return capacity_;
    }

    bool add_edge(size_t from, size_t to, T weight) {
        list_[from].emplace_back(Edge<T>(from, to, weight));
        return true;
    };

    // Dijkstra
    std::optional<std::pair<std::vector<size_t>, T>> dijkstra(size_t from, size_t to){
      const auto cmp = [](const std::pair<size_t, T>& a, const std::pair<size_t, T>& b) {
                         return a.second > b.second;
                       };
      std::priority_queue<
        std::pair<size_t, T>, 
        std::vector<std::pair<size_t, T>>, 
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
        auto cost = distances.find(e).second;
        if (current_node == to) {
          time_done = e.second;
          break;
        }
        
        for (auto& edge : list_[current_node]) {
          auto node_dist_iter = distances.find(make_pair(edge.to(), time));
          size_t node_dist = node_dist_iter =
            distances.end() ? std::numeric_limits<size_t>::max() : node_dist_iter->second;
          
          //in progress
          size_t edge_open_time = edge.starting_time() + (edge.period()/current_time)*edge.period();
          // Must wait
          if (edge_open_time > 0) {
            
          } else {
            
          }
        }

      }


      if (time_done == std::numeric_limits<size_t>::max())  {
        return std::nullopt;
      } else {
        std::vector<size_t> shortest_path;
        auto current = make_pair(to, time_done);
        
        auto start = make_pair(from, 0);
        while (current != start) {
          shortest_path.push_back(*came_from.find(current));
          current = *came_from.find(current);
        }
        std::reverse(shortest_path.begin(), shortest_path.end());
        return make_optional(std::make_pair(shortest_path, distances[to]));
      }
    };

  };
} // namespace popup
