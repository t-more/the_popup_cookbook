#include <iostream>
#include <vector>
#include <queue>
#include <optional>
#include <limits>

namespace popup {
    template <typename Flow, typename Weight>
    class Edge {
        size_t from_;
        size_t to_;
        Flow capacity_;
        Flow flow_ = 0;
        Weight weight_;
        Edge<Flow, Weight>* oposite_edge_;

    public:
        Edge(size_t from, size_t to, Flow capacity, Weight weight) {
            from_ = from;
            to_ = to;
            flow_ = 0;
            capacity_ = capacity;
            weight_ = weight;
        }

        Edge<Flow, Weight> make_oposite() {

        }

        void set_oposite(Edge<Flow, Weight>* oposite) {
            oposite_edge_ = oposite;
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
        }
        Flow residual() const {
            return capacity_ - flow_;
        }

        Edge<Flow, Weight>* oposite_edge() const {
            return oposite_edge_;
        }
    };

    template <typename Flow, typename Weight>
    class FlowGraph {
        std::vector<std::vector<Edge<Flow, Weight>>> list_;
        size_t num_nodes_ = 0;


    public:
        FlowGraph(size_t num_nodes) {
            num_nodes_ = num_nodes;
            list_.resize(num_nodes, std::vector<Edge<Flow, Weight>>());
        }

        // BFS to find the shortest path (in number of edges) from source to
        // sink. If this isn't possible it will return false, otherwie true.
        bool min_edge_path(size_t source, size_t sink, std::vector<Edge<Flow, Weight>*>& came_from) {

            std::vector<bool> visited(num_nodes_, false);
            if (came_from.size() < num_nodes_) {
                came_from.resize(num_nodes_);
            }

            std::queue<size_t> queue;
            queue.push(source);

            while (!queue.empty()) {
                auto current_node = queue.front();
                queue.pop();

                for (const auto& neighbor : list_[current_node]) {
                    if (neighbor == sink) {
                        came_from[sink] = current_node;
                        return true;
                    }
                    if (!visited[neighbor]) {
                        came_from[neighbor] = current_node;
                        queue.push(neighbor);
                    }
                }
            }
            return false;
        }

        // Modifies graph
        void edmond_karp(size_t source, size_t sink) {
            const auto& outbound_edges = list_[source];
            std::vector<Edge<Flow, Weight>*> came_from(num_nodes_, nullptr);

            while (min_edge_path(source, sink, came_from)) {
                Flow min_residual = std::numeric_limits<Flow>::max();
                size_t current_node = sink;
                while (current_node != source) {
                    const auto& edge = came_from[current_node];
                    min_residual = std::min(min_residual, edge.residual());
                }
            }
            for (const auto& e : outbound_edges) {


            }
        }


    };

} // namespace popup
