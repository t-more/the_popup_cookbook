// Authors: Tomas Möre, Marcus Östling 2019
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstring>

namespace popup {


  // Solves the knapsack problem. Accepts a random acces iterator of pairs
  // values (value, weight) and a positive integer capacity.  Will return a
  // vector of the indicies of the picked elements that maximize the value
  template <class RandomAccessIterator>
  std::vector<int> knapsack(int64_t capacity,
                            RandomAccessIterator items_begin,
                            RandomAccessIterator items_end)
  {

    size_t num_items = items_end - items_begin;
    std::vector<int> indices(num_items);
    std::iota(indices.begin(), indices.end(), 0);

    // Comparison function that proxies the indicies vector to the original
    auto compare = [&](const int& a_idx, const int& b_idx) {
                     const auto& a = *(items_begin + a_idx);
                     const auto& b = *(items_begin + b_idx);
                     return a.second < b.second;
                   };

    std::sort(indices.begin(), indices.end(), compare);

    // Cache of values all initialted to 0
    int64_t cache[indices.size() + 1][capacity + 1];
    std::memset(cache, 0, (indices.size() + 1) * (capacity + 1) * sizeof(int64_t));

    // Calculate the knapsack problem
    for (int i = 1; i <= num_items; i++) {
      for (int j = 0; j <= capacity; j++) {
        auto& item = *(items_begin + indices[i-1]);;
        if (item.second > j) {
          cache[i][j] = cache[i-1][j];
        } else {
          cache[i][j] = std::max(cache[i-1][j],
                                 cache[i-1][j-item.second] + item.first);
        }
      }
    }

    // Backtracks the knapsack table to get the chosen item indices.
    std::vector<int> res;

    int64_t weight = capacity;
    int64_t val = cache[indices.size()][capacity];

    for (int i = num_items; i > 0 && val > 0; i--) {
      //  If values differ then the item was selected
      if (val != cache[i-1][weight]) {

        auto& item = *(items_begin + indices[i-1]);
        res.push_back(indices[i-1]);
        val -= item.first;
        weight -= item.second;
      }
    }
    return res;
  }
}
