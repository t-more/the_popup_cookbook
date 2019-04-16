#include <bits/stdc++.h>
using namespace std;

unsigned long long mod(unsigned long long x, unsigned long long m) {
    return ((x % m) + m) % m;
}

int64_t mul_mod(uint64_t a, uint64_t b, uint64_t m) {
   uint64_t d = 0, mp2 = m >> 1;
   int i;
   if (a >= m) a %= m;
   if (b >= m) b %= m;
   for (i = 0; i < 64; ++i)
   {
       d = (d > mp2) ? (d << 1) - m : d << 1;
       if (a & 0x8000000000000000ULL)
           d += b;
       if (d >= m) d -= m;
       a <<= 1;
   }
   return d;
}

void compute_hash(
        string const& s,
        vector<unsigned long long> &hashes,
        unsigned long long m,
        unsigned long long p)
{
    unsigned long long hash_value = 0;
    unsigned long long p_pow = 1;
    for (char c : s) {
        //hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        hash_value = (hash_value + mul_mod(c - 'a' + 1, p_pow, m)) % m;
        hashes.push_back(hash_value);
        //p_pow = (p_pow * p) % m;
        p_pow = mul_mod(p_pow, p, m);
    }
}

int main() {
    cin.sync_with_stdio(0);
    cin.tie(0);

    string str;
    size_t q;
    cin >> str;
    cin >> q;

    const unsigned long long m = 922337220451ULL;
    const unsigned long long p = 16069ULL;
    vector<unsigned long long> inv(str.size());
    inv[0] = 1;
    //inv[1] = 838709685; // pow(2, 31) % 1e9+9
    inv[1] = 184249329619ULL; // pow(2, 31) % 1e9+9
    //inv[1] = 12; // pow(2, 31) % 53
    for(unsigned long long i = 2; i < str.size(); i++) {
        //inv[i] = (inv[i-1] * inv[1]) % m;
        inv[i] = mul_mod(inv[i-1], inv[1], m);
    }

    vector<unsigned long long> hashes;
    compute_hash(str, hashes, m, p);
    for(size_t cnt = 0; cnt < q; cnt++) {
        size_t i, j;
        cin >> i >> j;
        j--;
        if(i == 0) {
            cout << hashes[j]*inv[i]%m << "\n";
        } else {
            i--;
            //cerr << hashes[j] << endl;
            //cerr << hashes[i] << endl;
            cout << mul_mod((hashes[j]+m - hashes[i])%m, inv[i+1], m) << "\n";
            //cout << mul_mod(mod(hashes[j] - hashes[i], m), inv[i+1], m) << "\n";
        }
    }

    return 0;
}
