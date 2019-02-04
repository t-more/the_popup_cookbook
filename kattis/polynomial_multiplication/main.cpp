#include "polynomial_multiplication.hpp"
#include <cstdio>

int64_t next_int64() {
  char c = getchar_unlocked();
  char prefix_char = 0;
  while ('0' > c || '9' < c) {
    prefix_char = c;
    c = getchar_unlocked();
  }
  int x = 0;
  while ('0' <= c && c <= '9') {
    x = 10 * x + c - '0';
    c = getchar_unlocked();
  }
  return (((prefix_char != '-') << 1) - 1) * x;
}

int main() {
  int tests;
  tests = next_int64();
  
  int64_t p1size = next_int64();
  popup::Polynomial<double> p1(p1size+1);
  for(int i = 0; i <= p1size; i++)
    p1[i] = next_int64();

  int64_t p2size = next_int64();
  popup::Polynomial<double> p2(p2size+1);
  for(int i = 0; i <= p2size; i++)
    p2[i] = next_int64();

  auto res = p1.polymul(p2);
  size_t len_index = res.size()-1;
  while(len_index > 0 && std::round(res[len_index]) == 0) {
    len_index--;
  }
  printf("%lu\n", len_index);
  for(size_t i = 0; i < len_index+1; i++)
    printf("%d ", (int)std::round(res[i]));
  
  return 0;
}
