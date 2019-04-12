#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin, x.end()
#define sx(x) (int)(x).size()
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;

class Trie {
public:
    class vertex {
    public:
        bool leaf_ = false;
        int transition_[26];
        vertex() {
            fill(transition_, transition_ + 26, -1);
        };
    };

    std::vector<vertex> automaton = std::vector<vertex>(1);
    int current_index = 0;

    template <typename ForwardItr>
    void add_string(ForwardItr begin, ForwardItr end) {
        int idx = 0;

        for (auto itr = begin; itr != end; itr++) {
            int c = *itr - 'a';
            if (automaton[idx].transition_[c] == -1) {
                automaton[idx].transition_[c] = (int)automaton.size();
                automaton.emplace_back();
            }
            idx = automaton[idx].transition_[c];
        }

        automaton[idx].leaf_ = true;
    }
    void add_string(const std::string& str) {
        return add_string(str.cbegin(), str.cend());
    }
};


int best = 0;

void dfs(int pos, Trie& trie, int score, vector<int>& tile_count, vector<vector<int>>& point_count) {
    if (trie.automaton[pos].leaf_) {
        best = max(best,score);
    }
    for (int c = 0; c < 26;  c++) {
        int to_idx = trie.automaton[pos].transition_[c];
        if (to_idx != -1 && tile_count[c] > 0) {
            tile_count[c]--;
            auto here_score = point_count[c][tile_count[c]];
            dfs(to_idx, trie, here_score + score,  tile_count, point_count);
            tile_count[c]++;
        }
    }

}

int main() {
  cin.sync_with_stdio(0);
  cin.tie(0);
  cin.exceptions(cin.failbit);

  size_t num_words;
  cin >> num_words;
  Trie trie;


  for (int i = 0; i < num_words; i++) {
      string s;
      cin >> s;
      trie.add_string(s);
  }

  size_t hands;
  cin >> hands;
  while (hands--) {
      size_t tiles;
      cin >> tiles;
      vector<int> tile_count(26);
      vector<vector<int>> point_count(26);

      rep (i, 0, tiles) {
          char c;
          int value;
          cin >> c >> value;
          tile_count[c -'a'] += 1;
          point_count[c-'a'].push_back(value);
      }
      for (auto& v : point_count) {
          if (!v.empty()){
              sort(v.begin(),v.end());
          }
      }
      best = 0;
      dfs(0, trie, 0, tile_count, point_count);
      cout << best << "\n";
  }


}
