#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <stack>
#include <optional>
#include <limits>
#include <algorithm>
#include <cassert>
#include <functional>

namespace popup {
    template <typename Flow, typename Weight>
    class Edge {
        size_t from_;
        size_t to_;
        Flow capacity_;
        Flow flow_ = 0;
        Weight weight_;
        Edge<Flow, Weight>* opposite_edge_;

    public:
        Edge(size_t from, size_t to, Flow capacity, Weight weight) {
            from_ = from;
            to_ = to;
            flow_ = 0;
            capacity_ = capacity;
            weight_ = weight;
        }

        Edge<Flow, Weight>* make_opposite() {
            Edge<Flow, Weight>* e = new Edge<Flow, Weight>(to_, from_, 0, -weight_);
            e->set_opposite(this);
            set_opposite(e);
            return e;
        }

        void set_opposite(Edge<Flow, Weight>* opposite) {
            opposite_edge_ = opposite;
        }

        size_t from() const {
            return from_;
        }

        size_t to() const {
            return to_;
        }

        Flow capacity() const {
            return capacity_;
        }

        Flow flow() const {
            return flow_;
        }

        Flow weight() const {
            return weight_;
        }

        void increase_flow(Flow f) {
            flow_ += f;
            opposite_edge_->flow_ -= f;
        }

        Flow residual() const {
            return capacity_ - flow_;
        }

        Edge<Flow, Weight>* opposite_edge() const {
            return opposite_edge_;
        }
    };

    template <typename Flow, typename Weight>
    class FlowGraph {
        std::vector<std::vector<Edge<Flow, Weight>*>> list_;
        size_t num_nodes_ = 0;


    public:
        FlowGraph(size_t num_nodes) {
            num_nodes_ = num_nodes;
            list_.resize(num_nodes, std::vector<Edge<Flow, Weight>*>());
        }

        void add_edge(size_t from, size_t to, Flow capacity, Weight weight) {
            Edge<Flow, Weight>* edge = new Edge<Flow, Weight>(from, to, capacity, weight);
            list_[from].emplace_back(edge);
            list_[to].emplace_back(edge->make_opposite());
        }

        // Only to be used after edmond-karp
        std::vector<Edge<Flow, Weight>*> get_used_edges() {
            std::vector<Edge<Flow, Weight>*> used_edges;
            for(auto v : list_) {
                for(auto edge : v) {
                    if(edge->flow() > 0 && edge->capacity() > 0) {
                        used_edges.emplace_back(edge);
                    }
                }
            }

            return used_edges;
        }

        Flow outbound_flow(size_t source) {
            Flow total = 0;
            for(auto edge : list_[source]) {
                if(edge->capacity() > 0) {
                    total += edge->flow();
                }
            }
            return total;
        }


        void dfs(size_t start, std::function<void(size_t)> f) const {
            std::stack<size_t> stack;
            stack.push(start);
            std::vector<bool> visited(num_nodes_, false);
            while (!stack.empty()) {
                auto node = stack.top();
                stack.pop();
                f(node);
                visited[node] = true;

                for (auto& edge : list_[node]) {
                    if (!visited[edge->to()] && edge->residual() > 0) {
                        visited[edge->to()] = true;
                        stack.push(edge->to());
                    }

                }
            }
        };


        // BFS to find the shortest path (in number of edges) from source to
        // sink. If this isn't possible it will return false, otherwie true.
        std::pair<Flow, Weight> bfs(
                size_t source,
                size_t sink,
                std::vector<Edge<Flow, Weight>*> &came_from
                ) {
            std::vector<bool> visited(num_nodes_, false);
            std::vector<Flow> min_residual(num_nodes_, std::numeric_limits<Flow>::max());

            if (came_from.size() < num_nodes_) {
                came_from.resize(num_nodes_);
            }

            std::queue<size_t> queue;
            queue.push(source);

            while (!queue.empty()) {
                auto current_node = queue.front();
                queue.pop();
                visited[current_node] = true;

                for (const auto edge : list_[current_node]) {
                    if(edge->residual() <= 0) {
                        continue;
                    }

                    if (edge->to() == sink) {
                        came_from[sink] = edge;
                        return std::make_pair(std::min(min_residual[current_node], edge->residual()), 0);
                    }

                    if (!visited[edge->to()]) {
                        came_from[edge->to()] = edge;
                        min_residual[edge->to()] =
                            std::min(min_residual[current_node], edge->residual());
                        queue.push(edge->to());
                        visited[edge->to()] = true;
                    }
                }
            }
            return std::make_pair(0, 0);
        }
        std::pair<Flow, Weight> desopo_pape(
            size_t from,
            size_t to,
            std::vector<Edge<Flow, Weight>*> &came_from

        ) {
            std::vector<Weight> distances(
                num_nodes_,
                std::numeric_limits<Weight>::max()
            );
            std::vector<Flow> min_residual(
                num_nodes_,
                std::numeric_limits<Flow>::max()
            );
            distances[from] = 0;
            std::vector<int> status(num_nodes_, 2);
            std::deque<size_t> que;
            que.push_back(from);
            std::fill(came_from.begin(), came_from.end(), nullptr);

            while (!que.empty()) {
                size_t u = que.front();
                que.pop_front();
                status[u] = 0;

                for (auto& edge : list_[u]) {
                    if (edge->residual() > 0 && distances[edge->to()] > distances[u] + edge->weight()) {

                        distances[edge->to()] =  distances[u] + edge->weight();
                        came_from[edge->to()] = edge;
                         min_residual[edge->to()] = std::min(
                                min_residual[edge->from()],
                                edge->residual()
                            );
                        if (status[edge->to()] == 2) {
                            status[edge->to()] = 1;
                            que.push_back(edge->to());
                        } else if (status[edge->to()] == 0) {
                            status[edge->to()] = 1;
                            que.push_front(edge->to());
                        }
                    }
                }
            }
            if (min_residual[to] == std::numeric_limits<Flow>::max())  {
                return std::make_pair(0, 0);
            } else {
                return std::make_pair(min_residual[to], distances[to]);
            }
        }
        std::pair<Flow, Weight> bellman_ford(
            size_t from,
            size_t to,
            std::vector<Edge<Flow, Weight>*> &came_from
        ) {
            std::vector<Weight> distances(
                num_nodes_,
                std::numeric_limits<Weight>::max()
            );
            std::vector<Flow> min_residual(
                num_nodes_,
                std::numeric_limits<Flow>::max()
            );
            distances[from] = 0;

            Edge<Flow, Weight> dummy_edge =
                Edge<Flow, Weight>(0, from, std::numeric_limits<Flow>::max(), 0);
            came_from[from] = &dummy_edge;

            for (size_t n = 0; n < num_nodes_ - 1; n++) {
                for (size_t i = 0; i < num_nodes_; i++) {
                    for (auto& edge : list_[i]) {
                        if (distances[edge->from()] == std::numeric_limits<Weight>::max()) {
                            continue;
                        }
                        auto trav_cost = distances[edge->from()] + edge->weight();
                        if (trav_cost < distances[edge->to()] && edge->residual() > 0) {
                            came_from[edge->to()] = edge;
                            distances[edge->to()] = trav_cost;
                            min_residual[edge->to()] = std::min(
                                min_residual[edge->from()],
                                edge->residual()
                            );
                        }
                    }
                }
            }
            //std::cerr << "RUN\n" ;
            if (min_residual[to] == std::numeric_limits<Flow>::max())  {
                return std::make_pair(0, 0);
            } else {
                return std::make_pair(min_residual[to], distances[to]);
            }
        }

        std::pair<Flow, Weight> dijkstra(
            size_t from,
            size_t to,
            std::vector<Edge<Flow, Weight>*> &came_from
        ) {
            const auto cmp = [](const std::pair<Edge<Flow,Weight>*, Flow>& a,
                                const std::pair<Edge<Flow,Weight>*, Flow>& b) {
                                 return a.second > b.second;
                             };

            std::priority_queue<
                std::pair<Edge<Flow,Weight>*, Weight>,
                std::vector<std::pair<Edge<Flow,Weight>*, Weight>>,
                decltype(cmp)> queue(cmp);
            std::vector<Weight> distances(num_nodes_, std::numeric_limits<Weight>::max());
            std::vector<bool> visited(num_nodes_, false);
            std::vector<Flow> min_residual(num_nodes_, std::numeric_limits<Flow>::max());

            distances[from] = 0;
            Edge<Flow, Weight> dummy_edge =
                Edge<Flow, Weight>(0, from, std::numeric_limits<Flow>::max(), 0);
            //            dummy_edge.flow_ = std::numeric_limits<Flow>::max();
            queue.emplace(std::make_pair(&dummy_edge,0));
            came_from[from] = &dummy_edge;
            //min_residual[-1] = std::numeric_limits<int>::max();

            while (!queue.empty()) {
                auto e = queue.top();
                queue.pop();
                auto current_edge = e.first;
                auto current_node = current_edge->to();
                if (visited[current_node]) {
                    continue;
                }

                min_residual[current_node] = std::min(
                    min_residual[current_edge->from()],
                    current_edge->residual()
                );
                visited[current_node] = true;

                auto cost = e.second; //distances[current_node];

                if (current_node == to) {
                    break;
                }

                for (const auto edge : list_[current_node]) {
                    if (edge->residual() > 0 && !visited[edge->to()]) {
                        auto node = edge->to();
                        const auto weight = edge->weight();
                        auto node_dist = distances[node];
                        auto alt_dist = weight + cost;
                        //std::cerr << alt_dist << std::endl;
                        if (node_dist > alt_dist) {
                            distances[node] = alt_dist;
                            came_from[node] = edge;
                            queue.emplace(std::make_pair(edge, alt_dist));
                        }
                    }
                }
            }
            {
                auto ce = came_from[to];
                while (ce != nullptr && ce != &dummy_edge) {
                    //std::cerr << ce->to() << " ";
                    ce = came_from[ce->from()];
                }
                //std::cerr << "\n";
            }
            //std::cerr << "RUN\n" ;
            if (!visited[to])  {
                return std::make_pair(0, 0);
            } else {
                return std::make_pair(min_residual[to], distances[to]);
            }
        };



        // Modifies graph
        std::pair<Flow, Weight> ford_fulkerson(
            size_t source,
            size_t sink,
            std::function<std::pair<Flow,Weight>(
                size_t,
                size_t,
                std::vector<Edge<Flow, Weight>*>&)> path_algo
        ) {
            std::vector<Edge<Flow, Weight>*> came_from(num_nodes_, nullptr);
            Flow flow = 0;
            Weight cost = 0;
            for (std::pair<Flow, Weight> res = path_algo(source, sink, came_from);
                 res.first > 0;
                 res = path_algo(source, sink, came_from)) {

                size_t current_node = sink;
                flow += res.first;
                cost += res.first * res.second;
                //std::cerr << "rf: " << res.first <<  " rs: "<<res.second <<  std::endl;

                while (current_node != source) {
                    const auto edge = came_from[current_node];
                    edge->increase_flow(res.first);
                    current_node = edge->from();
                }
            }
            return std::make_pair(flow,cost);
        }


        std::pair<Flow, Weight> min_cost_max_flow(size_t source, size_t sink) {
            using namespace std::placeholders;
            return ford_fulkerson(
                source,
                sink,
                std::bind(&FlowGraph<Flow,Weight>::desopo_pape, this, _1, _2, _3)
            );
        }
        Flow edmond_karp(size_t source, size_t sink) {
            using namespace std::placeholders;
            return ford_fulkerson(
                source,
                sink,
                std::bind(&FlowGraph<Flow,Weight>::bfs, this, _1, _2, _3)
            ).first;
        }

        std::pair<std::vector<size_t>, Flow> st_cut(
            size_t source,
            size_t sink
        ) {
            Flow max_flow = edmond_karp(source, sink);
            std::vector<size_t> result;

            dfs(source, [&](size_t node) {
                            result.push_back(node);
                        });
            return std::make_pair(result, max_flow);
       }
    };

} // namespace popup
