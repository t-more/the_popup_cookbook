#include "knapsack.hpp"
#include <iostream>
#include <vector>
#include <utility>




int main() {
  double capacity;
  int num_items;

  while (std::cin >> capacity >> num_items) {
    std::vector<std::pair<int, int>> items(num_items);

    for (auto& item : items) {
      int value, weight;
      std::cin >> value >> weight;
      item = std::make_pair(value, weight);

    }

    auto result = popup::knapsack((int64_t)capacity,
                                  items.begin(),
                                  items.end());
    std::cout << result.size() << std::endl;
    for (auto idx : result) {
      std::cout << idx << " ";
    }
    std::cout << std::endl;
  }
}
