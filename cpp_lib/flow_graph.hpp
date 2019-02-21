#include <iostream>
#include <vector>
#include <queue>
#include <optional>
#include <limits>
#include <algorithm>

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
            Edge<Flow, Weight>* e = new Edge<Flow, Weight>(to_, from_, 0, weight_);
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

        // BFS to find the shortest path (in number of edges) from source to
        // sink. If this isn't possible it will return false, otherwie true.
        Flow min_edge_path(
                size_t source, 
                size_t sink, 
                std::vector<Edge<Flow, Weight>*>& came_from
                ) {
            static std::vector<bool> visited(num_nodes_, false);
            static std::vector<Flow> min_residual(num_nodes_, std::numeric_limits<Flow>::max());
            std::fill(visited.begin(), visited.end(), false);
            std::fill(min_residual.begin(), min_residual.end(), std::numeric_limits<Flow>::max());
            if (came_from.size() < num_nodes_) {
                came_from.resize(num_nodes_);
            }

            std::queue<size_t> queue;
            queue.push(source);

            while (!queue.empty()) {
                auto current_node = queue.front();
                queue.pop();

                for (const auto edge : list_[current_node]) {
                    if(edge->residual() <= 0) {
                        continue;
                    }

                    if (edge->to() == sink) {
                        came_from[sink] = edge;
                        return std::min(min_residual[current_node], edge->residual());
                    }

                    if (!visited[edge->to()]) {
                        came_from[edge->to()] = edge;
                        min_residual[edge->to()] = 
                            std::min(min_residual[current_node], edge->residual());
                        queue.push(edge->to());
                        visited[current_node] = true;
                    }
                }
            }
            return 0;
        }

        // Modifies graph
        void edmond_karp(size_t source, size_t sink) {
            std::vector<Edge<Flow, Weight>*> came_from(num_nodes_, nullptr);
            
            Flow min_residual;
            while ((min_residual = min_edge_path(source, sink, came_from))) {
                size_t current_node = sink;
                while (current_node != source) {
                    const auto edge = came_from[current_node];
                    edge->increase_flow(min_residual);
                    current_node = edge->from();
                }
            }
        }

    };

} // namespace popup

