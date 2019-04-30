#pragma once
#include "./vec.hpp"

#include <cassert>
#include <utility>
#include <iostream>
#include <limits>

namespace popup {

    template <unsigned int Dim, typename T>
    class LineSegment {

    };

    template <typename T>
    int sign(T n) {
        return (int)(T(0) < n) - (int)(n < T(0));
    }

    enum ClockOrder {
                     CW,
                     CCW
    };

    std::ostream& operator<<(std::ostream& os, ClockOrder o) {
        switch (o) {
        case CW:
            os << "CW";
        break;
        case CCW:
            os << "CCW";
            break;
        }
        return os;
    }

    template<typename T, typename RAItr>
    std::pair<ClockOrder, T> identify_order(RAItr begin, RAItr end) {
        assert((end - begin) >= 3);

        T sum = T();
        auto itr = begin;
        Vec2<T> first = *begin;

        Vec2<T> last = *begin;
        itr++;
        while (itr != end) {
            Vec2<T>& current = *(itr++);
            sum += (last[0] - current[0]) * (last[1] + current[1]);
            last = current;
        }
        sum += (last[0] - first[0]) * (last[1] + first[1]);
        sum /= 2.0;
        return {sum > 0 ? ClockOrder::CCW : ClockOrder::CW
                , std::abs(sum)};
    }

    template<typename T>
    bool interval_intersects_1d(T ae, T be, T ce, T de) {
        if (ae > be) std::swap(ae, be);
        if (ce > de) std::swap(ce, de);
        return std::max(ae, ce) <= std::min(be, de);
    };

    template<unsigned int Dim, typename T>
    bool interval_intersects(
        const Vec<Dim, T> &l1_begin,
        const Vec<Dim, T> &l1_end,
        const Vec<Dim, T> &l2_begin,
        const Vec<Dim, T> &l2_end
    ) {
        for (size_t i = 0; i < Dim; i++) {
            auto insertects_in_dim = interval_intersects_1d(
                l1_begin[i],
                l1_end[i],
                l2_begin[i],
                l2_end[i]);
            if (!insertects_in_dim) return false;
        }
        return true;
    };

    template<typename T>
    bool point_on_line(const Vec2<T>& point, Vec2<T> l1, Vec2<T> l2) {
        const double EPS = 1e-9;
        if (l2[0] < l1[0]) std::swap(l1, l2);
        return (l1[0] <= point[0] && point[0] < l2[0])
            && (std::abs(cross(point - l1, l2 - l1)) < EPS);
    }

    template<typename T>
    bool line_intersect(
            const Vec2<T> &l1_begin,
            const Vec2<T> &l1_end,
            const Vec2<T> &l2_begin,
            const Vec2<T> &l2_end) {
        const T EPS = 1e-9;
        if (std::abs(cross(l1_begin - l2_begin, l2_end - l2_begin)) < EPS &&
            std::abs(cross(l1_end   - l2_begin, l2_end - l2_begin)) < EPS) {
            return
                interval_intersects(
                    l1_begin, l1_end,
                    l2_begin, l2_end);
        }
        return
            (sign(cross(l1_end - l1_begin, l2_end - l1_begin)) !=
            sign(cross(l1_end - l1_begin, l2_begin - l1_begin))) &&
            (sign(cross(l2_end - l2_begin, l1_begin - l2_begin)) !=
            sign(cross(l2_end - l2_begin, l1_end - l2_begin)));
    }

    enum PointLocation {
        Outside = 0,
        Border,
        Inside
    };

    template<typename T, typename RAItr>
    PointLocation point_in_polygon(const Vec2<T>& point, RAItr begin, RAItr end) {
        const T EPS = 1e-9;
        const Vec2<T> outside = {{2e10, point[1]}};  // {{std::numeric_limits<T>::max(), point[1]}};
        size_t num_intersections = 0;
        auto prev = *begin;
        auto it = begin;
        it++;
        const auto count_intersections =
            [&](const Vec2<T> p1, const Vec2<T> p2) -> size_t {
                bool intersects = line_intersect(
                    point,
                    outside,
                    p1,
                    p2
                );
                auto control_point = p1;
                if (p2[1] < control_point[1]) {
                    control_point = p2;
                }
                if (!(std::abs(cross(control_point - point, outside - point)) < EPS)) {
                    return (size_t)intersects;
                } else {
                    return 0;
                }
            };
        for (; it != end; it++) {
            num_intersections += count_intersections(prev, *it);
            if (point_on_line(point, prev, *it)) {
                // std::cerr << point << " " << prev << " " << *it << std::endl;
                // std::cerr << "Here" << std::endl;
                return PointLocation::Border;
            }
            prev = *it;
        }

        num_intersections += count_intersections(prev, *begin);
        if (point_on_line(point,  prev, *begin)) {
            //            std::cerr << "Here too" << std::endl;
            return PointLocation::Border;
        }

        if (num_intersections % 2 == 1) {
            return PointLocation::Inside;
        } else {
            return PointLocation::Outside;
        }
    }

} // namespace popup
