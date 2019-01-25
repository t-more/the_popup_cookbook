#include <iostream>
#include <utility>
#include "interval_cover.hpp"

int main() {
  double left, right;
  size_t num_ranges;

  while (std::cin >> left >> right >> num_ranges) {
    std::vector<popup::range> ranges;

    // std::cerr << left << " " << right << " " << num_ranges << std::endl;
    for (int i = 0; i < num_ranges; i++) {
      double a, b;
      std::cin >> a >> b;
      // Experiment with push_back / emplace_back
      //      std::cerr << a <<":"<< b << std::endl;
      ranges.emplace_back(popup::make_range(a, b));
    }

    auto res = popup::interval_cover(left,
                                     right,
                                     ranges.begin(),
                                     ranges.end());
    if (!res.has_value()) {
      std::cout << "impossible\n";
    } else {
      auto vec = res.value();
      std::cout << vec.size() << std::endl;
      for (auto i : vec) {
        std::cout << i << " ";
      }
      std::cout << std::endl;
    }

  }
}
