#include <iostream>
#include <utility>
#include "interval_cover.hpp"

int main() {
  double left, right;
  size_t num_ranges;
  std::vector<popup::range> ranges;

  std::cin >> left >> right >> num_ranges;

  for (int i = 0; i < num_ranges; i++) {
    double a, b;
    std::cin >> a >> b;
    // Experiment with push_back / emplace_back
    ranges.emplace_back(popup::make_range(a, b));
  }
  auto res = popup::interval_cover(popup::make_range(left,right),
                                   ranges.begin(),
                                   ranges.end());
  if (res.empty()) {
    std::cout << "impossibe\n";
  } else {
    std::cout << res.size() << std::endl;
    for (auto i : res) {
      std::cout << i;
    }
  }


}
