#include <iostream>
#include "union_find.hpp"



int main() {
  size_t num_elements, num_operations;
  std::cin >> num_elements >> num_operations;
  popup::UnionFind uf(num_elements);

  for (int o = 0; o < num_operations; o++) {
    char op;
    size_t a, b;
    std::cin >> op >> a >> b;
    if (op == '?') {
      size_t root_a = uf.find(a);
      size_t root_b = uf.find(b);
      std::cout << ((root_a == root_b) ? "yes" : "no") << "\n";
    } else {
      uf.make_union(a, b);
    }
  }
  std::cout << std::endl;
}
