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
        const double EPS = 1e-15;
        if (l2[0] < l1[0]) std::swap(l1, l2);
        // std::cout << "Check cross: " << point << " " << l1 << " " << l2 << " "
        //     << std::abs(cross(point - l1, l2 - l1)) << std::endl;
        return (l1[0] <= point[0] && point[0] <= l2[0])
            && (std::min(l1[1], l2[1]) <= point[1] && point[1] <= std::max(l1[1],l2[1]))
            && (std::abs(cross(point - l1, l2 - l1)) < EPS);
    }

    template<typename T>
    bool line_intersect(
            const Vec2<T> &l1_begin,
            const Vec2<T> &l1_end,
            const Vec2<T> &l2_begin,
            const Vec2<T> &l2_end) {
        const T EPS = 1e-15;
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
        const T EPS = 1e-15;
        double angle_sum = 0;
        auto prev = *begin;
        auto it = begin;
        it++;
        for (; it != end; it++) {
            double cross_angle =
                cross((*it - point).normalized(), (prev - point).normalized());
            double dot_angle =
                (*it - point).normalized().dot((prev - point).normalized());
            angle_sum += std::atan2(cross_angle, dot_angle);
            if (point_on_line(point, prev, *it)) {
                return PointLocation::Border;
            }
            prev = *it;
        }

        double cross_angle =
            cross((*begin - point).normalized(), (prev - point).normalized());
        double dot_angle =
            (*begin - point).normalized().dot((prev - point).normalized());
        angle_sum += std::atan2(cross_angle, dot_angle);
        if (point_on_line(point,  prev, *begin)) {
            return PointLocation::Border;
        }

        if (std::abs(angle_sum) < EPS) {
            return PointLocation::Outside;
        } else {
            return PointLocation::Inside;
        }
    }
} // namespace popup
