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
        inline bool operator<(Edge<T> o) const {
            return weight_ < o.weight_;
        }
    };

    template<typename T>
    class BellmanFordResult {
        bool contains_cycles_;
        size_t start_node_;
        std::vector<T> distances_;
        std::vector<size_t> came_from_;
        std::vector<bool> inf_reachable_;
    public:
        BellmanFordResult(
            size_t start_node,
            bool contains_cycles,
            std::vector<T> distances,
            std::vector<size_t> came_from,
            std::vector<bool> inf_reachable) {

            start_node_ = start_node;
            contains_cycles_ = contains_cycles;
            distances_ = distances;
            came_from_ = came_from;
            inf_reachable_ = inf_reachable;
        }

        // Returns ture if the graph contains cycles in general.
        bool contains_cycles() const {
            return contains_cycles_;
        }
        bool contains_cycles(size_t to) const {
            return inf_reachable_[to];
        }
        size_t start_node() const {
            return start_node_;
        }

        // Returns true if it is possible to reache a node from the source node
        bool reachable(size_t node) const {
            return node < distances_.size() && distances_[node] != std::numeric_limits<T>::max();
        }

        // Returns the distance to a specific node if it is reachable and the
        // there is no negative cycle on the path.
        std::optional<T> distance_to(size_t node) const {
            if (!reachable(node) || inf_reachable_[node]) {
                return std::nullopt;
            } else {
                return std::make_optional(distances_[node]);
            }
        }

        // Reconstructs the path if there is a valid path without negative
        // cycles, otherwise the optional is empty.
        std::optional<std::vector<size_t>> reconstruct_path(size_t node) const {
            if (!reachable(node) || distance_to(distance_to).has_value()) {
                size_t current_node = node;
                std::vector<size_t> path;
                while (current_node != start_node()) {
                    path.push_back(came_from_[current_node]);
                    current_node = came_from_[current_node];
                }
                return std::make_optional(path);
            }
            return std::nullopt;
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
        std::unordered_map<size_t, std::shared_ptr<BellmanFordResult<T>>> cache_;

        std::vector<Edge<T>>& edges_from(size_t node) {
            return list_[node];
        }

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

        size_t num_nodes() const {
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

        // BFS that runs f on every node it visits. f is given two arguments,
        // first is the node when found and second is the distance to the given
        // node from the start.
        void bfs(size_t start, std::function<void(size_t node, size_t distance)> f) const {
            std::queue<std::pair<size_t, size_t>> queue;
            std::vector<bool> visited(num_nodes(), false);
            queue.push(std::make_pair(start, 0));

            while (!queue.empty()) {
                auto p = queue.front();
                queue.pop();
                auto current_node = p.first;
                f(current_node, p.second);
                visited[current_node] = true;

                for (auto& edge : list_[current_node]) {
                    if (!visited[edge.to()]) {
                        queue.push(std::make_pair(edge.to(), p.second+1));
                        visited[edge.to()] = true;
                    }
                }
            }
        }

        void dfs(size_t start, std::function<void(size_t)> f) const {
            std::stack<size_t> stack;
            stack.push(start);
            std::vector<bool> visited(num_nodes(), false);
            while (!stack.empty()) {
                auto node = stack.top();
                stack.pop();
                f(node);
                visited[node] = true;

                for (auto& edge : list_[node]) {
                    if (visited[edge.to()]) {
                        continue;
                    }
                    visited[edge.to()] = true;
                    stack.push(edge.to());
                }
            }
        };

        std::optional<std::pair<std::vector<size_t>, T>>
        dijkstra(size_t from, size_t to) const {
            const auto cmp = [](const std::pair<size_t, T>& a, const std::pair<size_t, T>& b) {
                                 return a.second > b.second;
                             };
            std::priority_queue<std::pair<size_t, T>, std::vector<std::pair<size_t, T>>, decltype(cmp)> queue(cmp);
            std::vector<T> distances(num_nodes(), std::numeric_limits<T>::max());
            // size_t::max() is the value representing that a node never has been viewed
            std::vector<size_t> came_from(num_nodes(), std::numeric_limits<size_t>::max());
            std::vector<bool> visited(num_nodes(), 0);

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
        std::shared_ptr<BellmanFordResult<T>>
        bellman_ford(size_t from) {
            // Asking for a element outside the graph
            auto cached = cache_.find(from);
            bool cache_exists = cached != cache_.end();
            if (!graph_modified_ && cache_exists) {
                return cached->second;
            }
            if (graph_modified_) {
                cache_.clear();
                graph_modified_ = false;
            }


            std::vector<T> distances(
                num_nodes(),
                std::numeric_limits<T>::max()
            );
            std::vector<size_t> came_from(
                num_nodes(),
                std::numeric_limits<size_t>::max()
            );

            distances[from] = 0;
            came_from[from] = from;

            // Main bellman-ford part
            for (int i = 0; i < ((int)num_nodes() - 1); i++) {
                for (int node = 0; node < (int)num_nodes(); node++) {
                    for (const auto& edge : list_[node]) {
                        if (distances[edge.from()] == std::numeric_limits<T>::max()) {
                            continue;
                        }
                        T trav_cost = distances[edge.from()] + edge.weight();
                      if (trav_cost < distances[edge.to()]) {
                          came_from[edge.to()] = edge.from();
                          distances[edge.to()] = trav_cost;
                      }
                  }
              }
          }

          bool found_negative_cycle = false;
          std::vector<bool> inf_reachable(num_nodes(), false);

          // Identify negative cycles, for each negative cycle found a dfs is
          // ran to find nodes that can be reached from it.
          for (int node = 0; node < (int)num_nodes(); node++) {
              for (const auto& edge : list_[node])  {
                  if (distances[edge.from()] == std::numeric_limits<T>::max()) {
                      continue;
                  }
                  T trav_cost = distances[edge.from()] + edge.weight();
                  if (trav_cost < distances[edge.to()] && !inf_reachable[edge.to()]) {
                      found_negative_cycle = true;
                      dfs(edge.to(), [&](size_t node){
                                         inf_reachable[node] = true;
                                     });
                  }
              }
          }

          std::shared_ptr<BellmanFordResult<T>> result =
              std::make_shared<BellmanFordResult<T>>(
                  from,
                  found_negative_cycle,
                  distances,
                  came_from,
                  inf_reachable
              );
          cache_.insert(
              std::make_pair(
                  from,
                  result
              ));
          return result;
      }


      std::vector<std::vector<T>> all_pairs_shortest_paths() {

          std::vector<std::vector<T>> result(num_nodes(), std::vector<T>(num_nodes(), std::numeric_limits<T>::max()));

          // Assing initial edge costs
          for (auto& inner : list_) {
              for (auto& edge : inner) {
                  result[edge.from()][edge.to()] = std::min(result[edge.from()][edge.to()], edge.weight());
              }
          }

          // Any non assigned self loops are kept
          for (int d = 0; d < num_nodes(); d++) {
              if (result[d][d] > 0) {
                  result[d][d] = 0;
              }
          }

          // Main meat of the floyd warshall
          for (int k = 0; k < num_nodes(); k++) {
              for (int i = 0; i < num_nodes(); i++) {
                  for (int j = 0; j < num_nodes(); j++) {
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
          popup::UnionFind uf(num_nodes());

          std::vector<Edge<T>> result(num_nodes() - 1);

          // Dis issss kruskaaaal
          int mst_edge_count = 0;
          T cost = 0;
          for (auto& edge : all_edges) {
              if (uf.find(edge.from()) != uf.find(edge.to())) {

                  cost += edge.weight();
                  uf.make_union(edge.from(), edge.to());
                  result[mst_edge_count++] = edge;
                  if (mst_edge_count >= num_nodes() -1) {
                      return std::make_pair(cost, result);
                  }
              }
          }
          return std::nullopt;
      }

      std::optional<std::vector<size_t>> eulerian_path_undirected() {
        size_t odd_index = 0;
        size_t odds[2] = {
            std::numeric_limits<size_t>::max(), 
            std::numeric_limits<size_t>::max()
        };

        for (auto &edges : list_) {
            size_t degree = list_.size();
            if (degree % 2 == 1) {
                odds[odd_index++] = edges[0].from(); 
            }
            if (odd_index > 2) {
                std::cerr << "Too many odds\n";
                return std::nullopt;
            }
        }

        bool odds_set[2] = {
            odds[0] != std::numeric_limits<size_t>::max(),
            odds[1] != std::numeric_limits<size_t>::max()
        };

        if(odds_set[0] ^ odds_set[1]){
            std::cerr << "Same odds\n";
            return std::nullopt;
        }

        if(odds_set[0] && odds_set[1]) {
            this->add_bi_edge(odds[0], odds[1], 0);
        }

        // Russian algorithm
        std::stack<size_t> stack;
        std::vector<size_t> result;
        std::unordered_set<void*> removed_edge;
        std::vector<bool> visited(capacity_);
        stack.push(0);
        while (!stack.empty()) {
            size_t v = stack.top();
            visited[v] = true;
            size_t degree = 0;
            for (auto &edge : list_[v]) {
                if(removed_edge.find((void*)&edge) == removed_edge.end()) {
                    degree++;
                }
            }
            std::cerr << v << " degree: " << degree << std::endl;
            if (degree == 0) {
                result.push_back(v);
                stack.pop();
            } else {
                for (auto &edge : list_[v]) {
                    if(removed_edge.find((void*)&edge) == removed_edge.end()) {
                        stack.push(edge.to());
                        removed_edge.insert((void*)(&edge));
                    }
                }
            }
        }
        
        for(bool v : visited) {
            if(!v) {
                std::cerr << "Did not visit every node " << v << std::endl;
                return std::nullopt;
            }
        }

        return result;
      }
    };

} // namespace popup
