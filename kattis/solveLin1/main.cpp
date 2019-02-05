#include <iostream>
#include <vector>
#include "lin_eq_solver.hpp"


int main() {
  int num_rows;
  while (std::cin >> num_rows && num_rows != 0) {
    std::vector<std::vector<double>> matrix =
      std::vector<std::vector<double>>(num_rows, std::vector<double>(num_rows));
    std::vector<double> b(num_rows);
    std::vector<double> x;
    for (auto& outer : matrix) {
      for (auto& inner : outer) {
        std::cin >> inner;
      }
    }
    for (auto& e : b) {
      std::cin >> e;
    }

    auto num_results = popup::linear_solve<double>(matrix, b, x);
    switch (num_results) {
    case popup::NONE:
      std::cout << "Inconsistent";
      break;
    case popup::ONE:
      for (auto& e : x) {
        std::cout << e << " ";
      }
      break;
    case popup::MULTIPLE:
      std::cout << "Multiple";
      break;
    }
    std::cout << "\n";
    // switch (result_type) {
    // case popup::NONE:

    //   break;
    // case popup::ONE:
    //   break;
    // case popup::MULTIPLE:
    //   break;
    // }
  }
}
