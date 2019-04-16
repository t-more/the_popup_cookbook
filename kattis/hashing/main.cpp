#include <bits/stdc++.h>
using namespace std;
char next_char() {
  char c = getchar_unlocked();
  while(c == 32 || c == 10) {
    c = getchar_unlocked();
  }
  return c;
}

uint64_t next_word64() {
  char c = getchar_unlocked();
  while ('0' > c || '9' < c) {
    c = getchar_unlocked();
  }
  int x = 0;
  while ('0' <= c && c <= '9') {
    x = 10 * x + c - 48;
    c = getchar_unlocked();
  }
  return x;
}

unsigned long long mod(unsigned long long x, unsigned long long m) {
    return ((x % m) + m) % m;
}

uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m)
{
   long double x;
   uint64_t c;
   int64_t r;
   if (a >= m) a %= m;
   if (b >= m) b %= m;
   x = a;
   c = x * b / m;
   r = (int64_t)(a * b - c * m) % (int64_t)m;
   return r < 0 ? r + m : r;
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
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        hashes.push_back(hash_value);
        //p_pow = (p_pow * p) % m;
        p_pow = p_pow *p % m ;
    }
}

int main() {
    string str;
    str.reserve(300000);


    char c = getchar_unlocked();
    while (c != '\n') {
        str.push_back(c);
        c = getchar_unlocked();
    }
    size_t q = next_word64();

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
    hashes.reserve(300000);
    compute_hash(str, hashes, m, p);
    for(size_t cnt = 0; cnt < q; cnt++) {
        size_t i = next_word64(), j = next_word64();
        j--;
        if(i == 0) {
            cout << hashes[j] << "\n";
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
