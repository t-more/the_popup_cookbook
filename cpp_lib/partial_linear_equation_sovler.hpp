// Authors: Tomas Möre, Marcus Östring
#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <limits>
#include <algorithm>
#include <numeric>

#define EPS 1e-12

namespace popup {

  enum SOLTUION_TYPE {
                 NONE = 0,
                 ONE = 1,
                 MULTIPLE = 2
  };

  template<typename T>
  SOLTUION_TYPE linear_solve(std::vector<std::vector<T>>& a,
                   std::vector<T>& b,
                   std::vector<T>& ans
                   ) {

    const auto num_rows = a.size();
    const auto num_columns = a[0].size();

    std::vector<int> is_used(num_columns, -1);

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

      if (std::fabs(a[row_max][pivot_column]) < EPS) {
        pivot_column++;
        continue;
      }

      std::swap(a[pivot_row], a[row_max]);
      std::swap(b[pivot_row], b[row_max]);

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

      rank++;
    }

    // for (auto& outer : a) {
    //   for (auto& inner : outer) {
    //     std::cout << inner << " ";
    //   }
    //   std::cout << "\n";
    // }

    // for (auto& inner : b) {
    //   std::cout << inner << " ";
    // }
    // std::cout << "\n";
    // std::cout << "Rank: " << rank << "\n";
    for (int row = num_rows-1; row >= rank; row--) {
      T sum = 0;
      for (int i = 0; i < num_columns; i++)
        sum += a[row][i];
      //      std::cout << "Sum: " << sum << "\n";
      if (std::fabs(sum - b[row]) > EPS) {
        return NONE;
      }
    }

    ans.assign(rank, 0);
    // Corrects the b values as it goes to correct
    for (int row = rank -1; row >= 0; row--) {
      b[row] /= a[row][row];
      ans[row] = b[row];
      for (int j = row - 1; j >= 0; j--) {
        b[j] -= a[j][row] * b[row];
      }
    }

    if (rank < num_rows) {
      return MULTIPLE;
    } else {
      return ONE;
    }
  }


}
