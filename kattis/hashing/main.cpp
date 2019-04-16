#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
#define EPS 10e-9

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

long long mod(long long x, long long m) {
    return ((x % m) + m) % m;
}

long long compute_hash(string const& s, vector<long long> &hashes) {
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    for (char c : s) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        hashes.push_back(hash_value);
        p_pow = (p_pow * p) % m;

    }
    return hash_value;
}

int main() {
    cin.sync_with_stdio(0);
    cin.tie(0);

    string str;
    int q;

    cin >> str;
    cin >> q;

    long long m = 1e9+9;
    long long p = 31;
    vector<long long> inv(str.size());
    inv[0] = 1;
    inv[1] = 838709685; // pow(2, 1e9+9) % m
    for(long long i = 2; i <= p; i++) {
        inv[i] = (inv[i-1] * inv[1]) % m;
    }

    vector<ll> hashes;
    compute_hash(str, hashes);
    cout << hashes.size() << endl;
    for(int cnt = 0; cnt < q; cnt++) {
        int i, j;
        cin >> i >> j;
        if(i == 0) {
            cout << hashes[j-1] << "\n";
        } else {
            cout << mod(mod(hashes[j-1] - hashes[i-2], m)*inv[i-1], m) << "\n";
        }
    }

    return 0;
}
