#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin, x.end()
#define sx(x) (int)(x).size()
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;


size_t n;
bool maze[8][8] ={{false}};
int path[8*8];
int path_size = 0;
int max_depth = 1;

bool is_out(int x, int y) {
    return (x == 0 || x == (n -1)) || (y == 0 || y == (n -1));
}

using positions = vector<pair<int,int>>;


void win() {
    for (int i = 0; i < path_size; i++) {
        switch (path[i]) {
        case 0:
            cout << "north\n";
            break;
        case 1:
            cout << "east\n";
            break;
        case 2:
            cout << "south\n";
            break;
        case 3:
            cout << "west\n";
            break;
        }
    }
    exit(0);
}
uint64_t mhash(positions& state) {
    uint64_t h = 0 ;

    for (auto& p : state) {
        h &= 1 << ((p.first - 1) + (p.second - 1) * 7);
    }
    return h;
}

const pair<int,int> dirs[4] = {{0,-1}, {1,0}, {0,1}, {-1,0}};
set<uint64_t> visited;

void dfs(int c_depth, positions& pos) {

    auto h = mhash(pos);
    if (visited.find(h) != visited.end()) {
            return;
    }
    visited.insert(h);

    if (c_depth > max_depth) {
        return;
    }
    positions left; // COPY
    for (auto p : pos) {
        if (!is_out(p.first, p.second)) {
            left.push_back(p);
        }
    }
    if (left.empty()) {
        win();
    }

    for (int i = 0; i < 4; i++) {
        if (path_size > 0 && path[path_size -1] == ((i + 2) % 4)) {
            continue;
        }
        path[path_size++] = i;
        positions next(left); // COPY
        bool any_can_move = false;
        for (auto& p : next) {
            auto [x,y] = p;
            int new_x, new_y;
            new_x = x + dirs[i].first;
            new_y = y + dirs[i].second;
            if (maze[new_x][new_y]) {
                any_can_move = true;
                p = {new_x, new_y};
            }
        }
        if (any_can_move) {
            dfs(c_depth + 1, next);
        }
        path_size--;
    }
}

int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);
  cin.exceptions(cin.failbit);

  cin >> n;

  positions positions;

  for (int y = 0; y < n; y++) {
      for (int x = 0; x < n; x++) {
          char c;
          cin >> c;
          if (c == '.') {
              maze[x][y] = true;
              if (!is_out(x, y)) {
                  positions.emplace_back(x,y);
              }
          }
      }
  }

  while (true) {
      dfs(0, positions);
      max_depth++;
  }
}
