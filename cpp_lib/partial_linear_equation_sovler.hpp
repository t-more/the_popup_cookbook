// Authors: Marcus Östling, Tomas Möre 2019
# include <iostream>
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

  /**
   *  Solves a linear equation given a matrix A and a vector b.
   *  Returns NONE if there does not exist a solution.
   *          One if the exists a solution.
   *          MULTIPLE if the exists multiple solutions.
   *
   *  The matrix a and the vector b will not be perserved.
   *  If ONE was returned the solution can be found in ans.
   *  If MUTLIPLE was returned the solution can be found in ans with
   *  the variables that has multiple possible variables as NAN..
   */
  template<typename T>
  SOLTUION_TYPE linear_solve(std::vector<std::vector<T>>& a,
                   std::vector<T>& b,
                   std::vector<T>& ans
                   ) {

    const auto num_rows = a.size();
    const auto num_columns = a[0].size();

    size_t pivot_row, pivot_column; // h, k
    pivot_row = pivot_column = 0;
    int rank = 0;

    //Gaussian elimination to row echolon form
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

      pivot_row++;
      pivot_column++;

      rank++;
    }

    // Check if a solution exists.
    for (int row = num_rows-1; row >= rank; row--) {
      T sum = 0;
      for (int i = 0; i < num_columns; i++)
        sum += a[row][i];
      if (std::fabs(sum - b[row]) > EPS) {
        return NONE;
      }
    }
    if (rank < num_rows) {

      // Continue to reduced row echolon form
      // To find the variable that have multiple possibilities.
      ans.assign(num_rows, NAN);
      for (int row = num_rows -1; row >= 0; row--) {

        int first_in_col_idx = -1;
        bool multiple_found = false;
        for (int col = 0; col < num_columns; col++) {
          if (std::fabs(a[row][col]) > EPS && first_in_col_idx == -1) {
            first_in_col_idx = col;
          } else if(std::fabs(a[row][col]) > EPS) {
            multiple_found = true;
            break;
          }
        }

        for (int j = row - 1; j >= 0; j--) {
          const auto factor = a[j][first_in_col_idx]
            / a[row][first_in_col_idx];
          b[j] -= b[row] * factor;
          for(int i = first_in_col_idx; i < num_columns; i++) {
            a[j][i] -= factor * a[row][i];
          }
        }

        // Value was only on a row.
        if (!multiple_found && first_in_col_idx != -1) {
          b[row] /= a[row][first_in_col_idx];
          ans[first_in_col_idx] = b[row];
        }
      }
      return MULTIPLE;
    }

    // Corrects the b values as it goes to correct
    // and assign it to ans
    // Back substitution (but only for b)
    ans.assign(rank, 0);
    for (int row = rank -1; row >= 0; row--) {
      b[row] /= a[row][row];
      ans[row] = b[row];
      for (int j = row - 1; j >= 0; j--) {
        b[j] -= a[j][row] * b[row];
      }
    }

    return ONE;
  }

}
