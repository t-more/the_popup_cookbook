#include <bits/stdc++.h>
#include "graph.hpp"
using namespace std;

int main() {
    cin.sync_with_stdio(0);
    cin.tie(0);
    cin.exceptions(cin.failbit);


    int64_t n, m;
    cin >> n >> m;
    while(n != 0) {
        popup::Graph<int> graph(n);
        while(m--) {
            int from, to;
            cin >> from >> to;
            graph.add_edge(from, to, 0);
        }
        auto res = graph.eulerian_path();
        if(res) {
            for(size_t i : res.value()) {
                cout << i << " ";
            }
            cout << "\n";
        } else {
            cout << "Impossible\n";
        }
        cin >> n >> m;
    }



    return 0;
}
