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

    string pattern;
    string str;
    while(getline(cin, pattern)) {
        getline(cin, str);
        vector<string> p;
        p.emplace_back(pattern);
        auto res = popup::get_occurences(
            str.begin(), 
            str.end(), 
            p.begin(), 
            p.end()
        );

        for(auto i : res) {
            cout << i << " ";
        }
        cout << "\n";
    }
    return 0;
}
