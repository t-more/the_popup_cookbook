// Authors: Tomas Möre, Marcus Östling
#include <utility>
#include <vector>
#include <algorithm>
#include <limits>
#include <optional>
#include <numeric>

namespace popup {

  using range = std::pair<double, double>;

  range make_range(double a, double b) { return std::make_pair(a,b); }


  /**
   * This function finds the indiceis for the minimum set of intervals that
   * covers the specified range given by |range_left| and |range_right|.
   * |ranges_start| and |ranges_end| should be some random access iterator over
   * pairs or doubles representing each interval. The values of the range will
   * not be modified.
   * If no matching interval can be found it will return an empty vector.
  */
  template <class RandomAccessIterator>
  std::optional<std::vector<size_t>>
  interval_cover( double range_left,
                  double range_right,
                  RandomAccessIterator ranges_begin,
                  RandomAccessIterator ranges_end) {
    std::vector<size_t> result;

    // Keeps track of the original indices. This is the only mo
    std::vector<size_t> indices(ranges_end - ranges_begin);
    std::iota(indices.begin(), indices.end(), 0);

    auto start = indices.begin();
    auto end = indices.end();

    // Comparison method for the intervals. First by left value and secondarily
    // by right.
    auto comp = [&](const size_t &a_idx, const size_t &b_idx) {
                  const range& a = *(ranges_begin + a_idx);
                  const range& b = *(ranges_begin + b_idx);

                  if (a.first == b.first) {
                    return a.second > b.second;
                  } else {
                    return a.first < b.first;
                  }
                };

    std::sort(start, end, comp);

    double left = range_left;
    auto current = start;

    // Do because if left == right it still can find an interval.
    do {
      range best_range = make_range(-std::numeric_limits<double>::infinity(),
                                    -std::numeric_limits<double>::infinity()
                                    );
      bool best_found = false;
      size_t best_idx = 0;

      // Finds the
      for (; current != end && (ranges_begin + (*current))->first <= left; current++) {
        auto r = *(ranges_begin + (*current));
        if (r.second > best_range.second) {
          best_range = r;
          best_found = true;
          best_idx = *current;
        }
      }
      if (best_found) {
        result.push_back(best_idx);
        left = best_range.second;
      } else {
        //
        return std::nullopt;
      }
    } while (left < range_right);

    return std::make_optional(result);
  }
}
