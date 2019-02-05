#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <limits>
#include <algorithm>
#include <numeric>

#define EPS 1e-12

namespace popup {
  enum NUM_SOLUTIONS {
                      NONE = 0,
                      ONE = 1,
                      MULTIPLE = 2
  };

  template<typename T>
  int linear_solve(std::vector<std::vector<T>>& a,
                   std::vector<T>& b,
                   std::vector<T>& ans
                   ) {

    const auto num_rows = a.size();
    const auto num_columns = a[0].size();

    std::vector<int> is_used(num_columns, -1);
    std::vector<int> cols(num_columns);
    std::iota(cols.begin(), cols.end(), 0);

    size_t pivot_row, pivot_column; // h, k
    pivot_row = pivot_column = 0;
    int rank = 0;


    while (pivot_row < num_rows && pivot_column < num_columns) {
      size_t row_max = pivot_row;
      T max_found = std::fabs(a[pivot_row][pivot_column]);

      for (size_t i = pivot_row + 1; i < num_rows; i++) {
        auto value = std::fabs(a[i][pivot_column]);
        if (value > max_found) {
          row_max = i;
          max_found = value;
        }
      }

      if (a[row_max][pivot_column] < EPS) {
        pivot_column++;
        continue;
      } else {
        std::swap(a[pivot_row], a[row_max]);
        std::swap(b[pivot_row], b[row_max]);
        std::swap(cols[pivot_row], cols[row_max]);

        for (int i = pivot_row + 1; i < num_rows; i++) {
          const auto factor = a[i][pivot_column] / a[pivot_row][pivot_column];
          a[i][pivot_column] = 0;

          for (int j = pivot_column + 1; j < num_columns; j++) {
            a[i][j] -= factor * a[pivot_row][j];
          }
          b[i] -= factor * b[pivot_row];
        }


        is_used[pivot_column] = pivot_row;

        pivot_row++;
        pivot_column++;
      }
      rank++;
    }

    // for (auto& outer : a) {
    //   for (auto& inner : outer) {
    //     std::cout << inner << " ";
    //   }
    //   std::cout << "\n";
    // }

    ans.assign(rank, 0);

    // Corrects the b values as it goes to correct
    for (int i = rank -1; i >= 0; i--) {
      ans[cols[i]] = b[i];
      b[i] /= a[i][i];
      for (int j = i - 1; j >= 0; j--) {
        b[j] -= a[j][i] * b[i];
      }
    }


    for (int i = 0; i < num_rows; i++) {
      double sum = 0;
      for (int j = 0; j < num_columns; j++) {
        sum += ans[cols[j]] * a[i][j];
      }

      // If the sum of the row and b is above EPS then. There are no solution.
      if (std::abs(sum - b[i]) > EPS) {
        std::cout << "POTATO: " << std::abs(sum - b[i]) << std::endl;
        return 0;
      }
    }

    return (-(rank == num_rows)) + 2;
  }
}
