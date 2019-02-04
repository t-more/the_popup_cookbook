#include <vector>
#include <cmath>
#include <utility>
#include <limits>






template<typename T>
void linear_solve(std::vector<std::vector<T>>& a, std::vector<T>& b) {
  const auto num_rows = a.size();
  const auto num_columns = a[0].size();

  size_t pivot_row, pivot_column; // h, k
  pivot_row = pivot_column = 1;
  int rank = 0;
  while (pivot_row <= num_rows && pivot_column <= num_columns) {
    size_t row_max = 0;
    T max_found = -1;
    for (size_t i = pivot_row; i < num_rows; i++) {
      auto value = std::fabs(a[i][pivot_column]);
      if (value > max_found) {
        row_max = i;
        max_found = value;
      }
    }

    if (a[row_max][pivot_column] == 0) {
      pivot_column++;
      continue;
    } else {
      std::swap(a.begin() + pivot_row, a.begin() + row_max);
      std::swap(b.begin() + pivot_row, b.begin() + row_max);
      for (int i = pivot_row; i < num_rows; i++) {
        const auto factor = a[i][pivot_column] / a[pivot_row][pivot_column];

        a[i][pivot_column] = 0;

        for (int j = pivot_column; j < num_columns; j++) {
          a[i][j] -= a[pivot_row][j] * factor;
        }
      }
      pivot_row++;
      pivot_column++;
    }
    rank++;
  }

}
