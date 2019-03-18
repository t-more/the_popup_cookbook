#include <bits/stdc++.h>
#include "substring_search.hpp"
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
#define EPS 10e-9

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int main() {
    cin.sync_with_stdio(0);
    cin.tie(0);

    int num_patterns;
    while(cin >> num_patterns) {
        cin.ignore();
        vector<string> patterns;
        vector<vector<int>> result(num_patterns);
        string text;
        unordered_map<string, vector<int>> copy_of;

        string pattern;
        for(int i = 0; i < num_patterns; i++) {
            getline(cin, pattern);
            if(copy_of.find(pattern) == copy_of.end()) {
                patterns.push_back(pattern);
                copy_of[pattern] = vector<int>();
            }
            copy_of[pattern].push_back(i);
        }

        getline(cin, text);
        auto res = popup::get_occurences(
            text.begin(), text.end(), 
            patterns.begin(), patterns.end()
        );

        for (auto [pattern_idx, text_idx] : res) {
            string str = patterns[pattern_idx];
            for(auto i : copy_of[str]) {
                result[i].push_back(text_idx);
            }
        }

        for (auto &outer: result) {
            for (int i : outer) {
                cout << i << " ";
            }
            cout << "\n";
        }

    }

    return 0;
}
