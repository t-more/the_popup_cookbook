#include <iostream>
#include "flow_graph.hpp"
using namespace std;

int main() {
    cin.sync_with_stdio(0);
    cin.tie(0);

    int64_t num_nodes, num_edges, source, sink;
    cin >> num_nodes >> num_edges >> source >> sink;

    popup::FlowGraph<int64_t, int64_t> graph(num_nodes);
    int64_t from, to, capacity;
    while(num_edges--) {
        cin >> from >> to >> capacity;
        graph.add_edge(from, to, capacity, 0);
    }

    graph.edmond_karp(source, sink);
    auto used_edges = graph.get_used_edges();
    auto total = graph.outbound_flow(source);
    cout << num_nodes << " " << total << " " << used_edges.size() << "\n";
    for(auto edge : used_edges) {
        cout << edge->from() << " " << edge->to() << " " << edge->flow() << "\n";
    }

    return 0;
}
