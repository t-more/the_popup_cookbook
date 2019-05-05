#include <vector>
#include "vec.hpp"

namespace popup {

    template <typename T>
    bool ccw_turn(Point<2, T> a, Point<2, T> b, Point<2, T> c) {
        return cross(Vec2<T>(c-a), Vec2<T>(b-a)) < 0;
    }

    template <typename T>
    bool cw_turn(Point<2, T> a, Point<2, T> b, Point<2, T> c) {
        return cross(Vec2<T>(c-a), Vec2<T>(b-a)) > 0;
    }

    template <typename T, typename RAitr>
    std::vector<Point<2, T>> convex_hull(RAitr begin, RAitr end) {
        const auto cmp =
            [](const Point<2, T>& a, const Point<2, T>& b) {
                return a[0] < b[0] || (a[0] == b[0] && a[1] < b[1]);
            };

        sort(begin, end, cmp);

        Point2<T> start = *begin;
        Point2<T> mid_point = *(end-1);

        std::vector<Point2<T>> lower_hull;
        std::vector<Point2<T>> upper_hull;

        lower_hull.push_back(start);
        upper_hull.push_back(start);

        for (auto itr = begin+1; itr != end; itr++) {

            if (*itr == *(itr -1)) {
                continue;
            }

            if (itr == end - 1 || cw_turn(start, *itr, mid_point)) {
                while (upper_hull.size() >= 2
                       && !cw_turn(upper_hull[upper_hull.size() - 2],
                                   upper_hull[upper_hull.size() - 1],
                                   *itr)
                ) {
                    upper_hull.pop_back();
                }
                upper_hull.push_back(*itr);
            }

            if (itr == end - 1 || ccw_turn(start, *itr, mid_point)) {
                while (lower_hull.size() >= 2
                       && !ccw_turn(lower_hull[lower_hull.size() - 2],
                                   lower_hull[lower_hull.size() - 1],
                                   *itr)
                ) {
                    lower_hull.pop_back();
                }
                lower_hull.push_back(*itr);
            }
        }

        std::vector<Point2<T>> result;
        result.reserve(lower_hull.size() + upper_hull.size() - 2);
        for (auto& p : lower_hull) {
            result.push_back(p);
        }
        if (upper_hull.size() > 2) {
            for (auto itr = upper_hull.end()-2; itr != upper_hull.begin(); itr--) {
                result.push_back(*itr);
            }
        }
        return result;
    }

} // namespace popup
