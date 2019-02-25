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
    auto res = graph.st_cut(source,sink);
    cout << res.first.size() << "\n";
    for(auto edge : res.first) {
        cout << edge << "\n";
    }
    cout << "\n";

    return 0;
}
