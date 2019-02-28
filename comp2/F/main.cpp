#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin, x.end()
#define sx(x) (int)(x).size()
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;

void tarjan_connect(
    vector<vector<size_t>>& g,
    vector<size_t>& index,
    vector<size_t>& low_link,
    vector<bool>& on_stack,
    size_t& next_id,
    stack<size_t>& stack,
    vector<vector<size_t>>& components,
    size_t node
) {
    index[node] = next_id++;
    low_link[node] = index[node];
    stack.push(node);
    on_stack[node] = true;

    for (auto& to : g[node]) {
        if (index[to] == numeric_limits<size_t>::max()) {
            tarjan_connect(g,index,low_link,on_stack,next_id,stack, components, to);
            low_link[node] = min(low_link[node], low_link[to]);
        } else if (on_stack[to]) {
            low_link[node] = min(low_link[node], index[to]);
        }
    }


    if (low_link[node] == index[node]) {
        size_t t;
        components.push_back(vector<size_t>());
        auto& v = components[components.size() - 1];
        do {
            t = stack.top();
            stack.pop();
            on_stack[t] = false;
            v.push_back(t);
        } while (t != node);
        sort(v.begin(), v.end());
    }
}


vector<vector<size_t>> tarjan(vector<vector<size_t>>& g) {
    vector<size_t> index(g.size(), numeric_limits<size_t>::max());
    vector<size_t> low_link(g.size(), 0);
    vector<bool> on_stack(g.size(), false);
    stack<size_t> stack;
    size_t next_id = 0;
    vector<vector<size_t>> components;

    for (int node = 0; node < g.size(); node++) {
        if (index[node] == numeric_limits<size_t>::max()) {
            tarjan_connect(
                g,
                index,
                low_link,
                on_stack,
                next_id,
                stack,
                components,
                node
            );
        }
    }
    return components;
}

int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);
  cin.exceptions(cin.failbit);

  size_t num_rows;
  while (cin >> num_rows && num_rows != 0) {
      vector<vector<size_t>> graph(30, vector<size_t>());
      vector<bool> in_inpt(30,false);

      rep(r, 0, num_rows) {
          char row[5];
          cin >> row[0] >> row[1] >> row[2] >> row[3] >> row[4];
          char selected;
          cin >> selected;

          rep (i, 0, 5) {
              in_inpt[row[i] - 'A'] = true;
              graph[selected - 'A'].push_back(row[i] - 'A');
          }
      }
      auto res = tarjan(graph);
      sort(res.begin(), res.end(), [&](const vector<size_t>& a,
                                       const vector<size_t>& b) {
                                       if (a.empty()) {
                                           return false;
                                       }
                                       if (b.empty()) {
                                           return true;
                                       }
                                       return a[0] < b[0];
                                   });
      for (auto& r : res) {
          if (r.empty() || !in_inpt[r[0]]) {
              continue;
          }
          for (auto c : r) {
              cout << (char)('A' + c) << " ";
          }
          cout << endl;
      }
      cout << endl;
  }




}
