#include <bits/stdc++.h>
#include "hashing.hpp"

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

int main() {
    string str;
    str.reserve(300000);


    char c = getchar_unlocked();
    while (c != '\n') {
        str.push_back(c);
        c = getchar_unlocked();
    }
    size_t q = next_word64();

    popup::RollingHash rh(str);
    for(size_t cnt = 0; cnt < q; cnt++) {
        size_t i = next_word64(), j = next_word64();
        cout << rh.subhash(i, j) << endl;
    }

    return 0;
}
