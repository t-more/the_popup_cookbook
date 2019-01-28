#pragma once

#include <cstring>
#include <stack>

namespace popup {
  class UnionFind {

    struct Subset {
      size_t parent;
      int rank;
    };

    Subset* contents_;
    size_t num_elements_;
  public:
    UnionFind(size_t num_elements) {
      contents_ = new Subset[num_elements];
      for (size_t i = 0; i < num_elements; i++) {
        contents_[i] = {i, 0};
      }
      num_elements_ = num_elements;
    }
    ~UnionFind() {
      delete contents_;
    }

    size_t size() {
      return num_elements_;
    }


    size_t find(size_t a) {
      while (a != contents_[a].parent) {
        contents_[a].parent = contents_[contents_[a].parent].parent;
        a = contents_[a].parent;
      }
      return a;
      // std::stack<size_t> s;
      // auto current = a;

      // while (contents_[current].parent != current) {
      //   s.push(current);
      //   current = contents_[current].parent;
      // }

      // while (!s.empty()) {
      //   contents_[s.top()].parent = current;
      //   s.pop();
      // }
      // return current;
    }

    void make_union(size_t a, size_t b) {
      size_t root_a = find(a);
      size_t root_b = find(b);

      if (contents_[root_a].rank < contents_[root_b].rank) {
        contents_[root_a].parent = root_b;
      } else if (contents_[root_a].rank > contents_[root_b].rank) {
        contents_[root_b].parent = root_a;
      } else {
        contents_[root_b].parent = root_a;
        contents_[root_a].rank += 1;
      }
    };
  };

}
