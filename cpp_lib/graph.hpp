// Author: Tomas Möre, Marcus Östling 2019
#include <functional>
#include <optional>
#include <vector>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "union_find.hpp"

namespace popup {

    /**
     * Edge class used internaly for for the below graph class. Contains
     * information about where and to the edges go as well as a weight.
     */
    template <class T>
    class Edge {
        size_t from_ = 0;
        size_t to_ = 0;
        T weight_ = 0;

    public:
        Edge() {}
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

    /**
     *  The bellan ford result class stores information about a run of bellman
     *  ford. This ensures that computations aren't run that isn't required.
     */
    template<typename T>
    class BellmanFordResult {
        // Flag for checking if the result contains any cycles.
        bool contains_cycles_;
        // The starting node associated with this result
        size_t start_node_;
        // Vector of distances to nodes from the starting node
        std::vector<T> distances_;
        // Vector of nodes that were predecessors to the current node in found
        // path. OBS: there may be infinite cycles.
        std::vector<size_t> came_from_;
        // Vector to keep track of what nodes are reachable from a infinite
        // cycle that is reachable.
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

        // Returns true if the result some cycle.
        bool contains_cycles() const {
            return contains_cycles_;
        }
        // Returns true if there exists a negative cycle that is reachable from
        // both the starting node and the end node.
        bool contains_cycles(size_t to) const {
            return inf_reachable_[to];
        }

        // Returns the starting node associated with the result.
        size_t start_node() const {
            return start_node_;
        }

        // Returns true if it is possible to reach a node from the source node
        bool reachable(size_t node) const {
            return node < distances_.size()
                && distances_[node] != std::numeric_limits<T>::max();
        }

        // Returns the distance to a specific node if it is reachable and the
        // there is no negative cycle on the path between the stat node and the
        // target. Otherwise the result is empty.
        std::optional<T> distance_to(size_t node) const {
            if (!reachable(node) || inf_reachable_[node]) {
                return std::nullopt;
            } else {
                return std::make_optional(distances_[node]);
            }
        }

        // Reconstructs the path if there is a valid path without negative
        // cycles, otherwise the result is empty.
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

    /**
     *  A class representing weighted graphs.
     */
    template <class T>
    class Graph {
    protected:
        // Number of nodes in the graph.
        size_t size_ = 0;
        // Number of inserted edges in the graph
        size_t num_edges_ = 0;
        // Vector of edges originating from each node.
        std::vector<std::vector<Edge<T>>> list_;

        // Flag for keeping track of modifications in the graph for possobile
        // caching of results
        bool graph_modified_ = false;

        // Map keeping track of previous results of bellman ford runs.
        std::unordered_map<
            size_t,
            std::shared_ptr<BellmanFordResult<T>>
        > bellman_ford_cache_;
    public:
        /**
         * Internal method to get edges originating for a particular node
         */
        std::vector<Edge<T>>& edges_from(size_t node) {
            return list_[node];
        }



        Graph(size_t size) {
            size_ = size;
            list_ = std::vector<std::vector<Edge<T>>>(
                Graph<T>::size_,
                std::vector<Edge<T>>()
            );
        }

        /**
         *  Method that traverses all edges.
         */
        void traverse_edges(std::function<void(Edge<T>&)> f) {
            for(auto &outer : list_) {
                for(auto &inner : outer) {
                    f(inner);
                }
            }
        };

        /**
         * Adds two edges to the graph
         */
        void add_bi_edge(size_t a, size_t b, T weight) {
            add_edge(a, b, weight);
            add_edge(b, a, weight);
        };

        /**
         * Adds an edge to the graph. Will invalidate any caches.
         */
        void add_edge(size_t from, size_t to, T weight) {
            graph_modified_ = true;
            list_[from].emplace_back(Edge<T>(from, to, weight));
            num_edges_++;
        };

        size_t num_nodes() const {
            return size_;
        }
        size_t num_edges() const {
            return num_edges_;
        }

        /**
         * This part specifies generic algorithms on graphs
         * BFS that runs f on every node it visits. f is given two arguments,
         * first is the node when found and second is the distance to the given
         * node from the start.
         */
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

        /**
         * Standardied DFS. Will run the function f each time a node is visited
         */
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


        std::vector<T>
        dijkstra_dist(size_t from) const {
            const auto cmp = [](
                const std::pair<size_t, T>& a,
                const std::pair<size_t, T>& b
            ) {
                                 return a.second > b.second;
                             };
            std::priority_queue<
                std::pair<size_t, T>,
                std::vector<std::pair<size_t, T>>,
                decltype(cmp)
                > queue(cmp);
            std::vector<T> distances(num_nodes(), std::numeric_limits<T>::max());
            // size_t::max() is the value representing that a node never has been viewed
            std::vector<size_t> came_from(
                num_nodes(),
                std::numeric_limits<size_t>::max()
            );
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
            return distances;
        };

        /**
         * Standard dijkstra on the path. May not be run on graphs with negative
         * weighs.  If it can reach the target the value returns a pair of the
         * path and the total weight of the path. If there are no path the
         * result is empty.
         */
        std::optional<std::pair<std::vector<size_t>, T>>
        dijkstra(size_t from, size_t to) const {
            const auto cmp = [](
                const std::pair<size_t, T>& a,
                const std::pair<size_t, T>& b
            ) {
                return a.second > b.second;
            };
            std::priority_queue<
                std::pair<size_t, T>,
                std::vector<std::pair<size_t, T>>,
                decltype(cmp)
            > queue(cmp);
            std::vector<T> distances(num_nodes(), std::numeric_limits<T>::max());
            // size_t::max() is the value representing that a node never has been viewed
            std::vector<size_t> came_from(
                num_nodes(),
                std::numeric_limits<size_t>::max()
            );
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

        /**
        * Bellman ford returns a shared_ptr to a BellmanFordResult. If the
        * boolean is set to true if there simply was no path. It is false if a
        * negative cycle was detected.  If the pair is returned it contains the
        * shortest path as a vector and the weight of going there.
        */
        std::shared_ptr<BellmanFordResult<T>>
        bellman_ford(size_t from) {
            // Asking for a element outside the graph
            auto cached = bellman_ford_cache_.find(from);
            bool cache_exists = cached != bellman_ford_cache_.end();
            if (!graph_modified_ && cache_exists) {
                return cached->second;
            }
            if (graph_modified_) {
                bellman_ford_cache_.clear();
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
            bellman_ford_cache_.insert(
                std::make_pair(
                    from,
                    result
                ));
            return result;
        }


        std::vector<std::vector<T>> all_pairs_shortest_paths() {

            std::vector<std::vector<T>> result(
                num_nodes(),
                std::vector<T>(num_nodes(), std::numeric_limits<T>::max())
            );

            // Assing initial edge costs
            for (auto& inner : list_) {
                for (auto& edge : inner) {
                    result[edge.from()][edge.to()] =
                        std::min(result[edge.from()][edge.to()], edge.weight());
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
                            result[i][j] = std::min(
                                result[i][j],
                                result[i][k] + result[k][j]
                            );
                        }
                    }
                }
            }
            return result;
        }

        /**
         *  Kurskal's algorithm
         *  Returns the minimum spanning tree and its cost if one is found,
         *  else an empty result.
         */
        std::optional<std::pair<T,std::vector<Edge<T>>>> kruskal() {
            std::vector<Edge<T>> all_edges(num_edges());
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

        /**
         * Returns an eulerian cycle/path if it exists otherwise empty.  If an
         * eulerian cycle exists the first and last node of the cycle will be
         * the same. Hierholzer's algorithm.
         */
        std::optional<std::vector<size_t>> eulerian_path() {
            size_t odds_count = 0;
            size_t odds[2] = {
                              std::numeric_limits<size_t>::max(),
                              std::numeric_limits<size_t>::max()
            };

            std::vector<size_t> in_degree(num_nodes());
            std::vector<size_t> visited(num_nodes());

            for (auto &edges : list_) {
                for(auto &edge : edges) {
                    in_degree[edge.to()]++;
                }
            }

            for(size_t i = 0; i < in_degree.size(); i++) {
                size_t out_degree = list_[i].size();
                if(in_degree[i] < out_degree) {
                    odds[0] = i;
                    odds_count++;
                } else if(in_degree[i] > out_degree) {
                    odds[1] = i;
                    odds_count++;
                }

                if (odds_count > 2 || in_degree[i] >= out_degree + 2
                    || in_degree[i] + 2 <= out_degree) {
                    return std::nullopt;
                }
            }

            bool odds_set[2] = {
                                odds[0] != std::numeric_limits<size_t>::max(),
                                odds[1] != std::numeric_limits<size_t>::max()
            };

            if(odds_set[0] ^ odds_set[1]){
                return std::nullopt;
            }

            bool missing_edge = odds_set[0] && odds_set[1];

            std::vector<size_t> path;
            path.reserve(num_nodes());
            std::unordered_set<void*> removed_edges;
            std::stack<size_t> stack;

            // Idea shamelessy stolen from kactl
            std::vector< decltype(list_[0].begin()) > edges_left;
            edges_left.reserve(num_nodes());
            for (auto &edges : list_) {
                edges_left.push_back(edges.begin());
            }
            stack.push(missing_edge ? odds[0] : 0);

            while (!stack.empty()) {
                auto elem = stack.top();
                auto& edge_itr = edges_left[elem];
                auto edge_itr_end = list_[elem].end();
                while (edge_itr != edge_itr_end
                       && removed_edges.find((void*)&(*edge_itr)) != removed_edges.end()) {
                    edge_itr++;
                }
                if (edge_itr == edge_itr_end) {
                    path.push_back(elem);
                    stack.pop();
                } else {
                    stack.push(edge_itr->to());
                    removed_edges.insert((void*)&(*edge_itr));
                }
            }

            if (path.size() != num_edges() + 1) {
                return std::nullopt;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
    };

} // namespace popup
