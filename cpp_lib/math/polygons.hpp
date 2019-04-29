#pragma once
#include "./vec.hpp"

#include <cassert>
#include <utility>
#include <iostream>
#include <limits>

namespace popup {
    
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
            sum += (last[0] - current[0]) * (last[1] + current[1]) / 2.0;
            last = current;
        }
        sum += (last[0] - first[0]) * (last[1] + first[1]) / 2.0;

        return {sum > 0 ? ClockOrder::CCW : ClockOrder::CW
                , std::abs(sum)};
    }

    template<typename T>
    bool line_intersect(
            const Vec2<T> &l1_begin, 
            const Vec2<T> &l1_end, 
            const Vec2<T> &l2_begin, 
            const Vec2<T> &l2_end) {
        const T EPS = 1e-9;
        const auto interval_intersect = [](T ae, T be, T ce, T de) {
            if (ae > be) std::swap(ae, be);
            if (ce > de) std::swap(ce, de);
            return std::max(ae, ce) <= std::min(be, de);
        };
        if (std::abs(cross(l1_begin - l2_begin, l2_end - l2_begin)) < EPS && 
            std::abs(cross(l1_end   - l2_begin, l2_end - l2_begin)) < EPS) {
            return
                interval_intersect(
                        l1_begin[0], 
                        l1_end[0], 
                        l2_begin[0], 
                        l2_end[0]) &&
                interval_intersect(
                        l1_begin[1], 
                        l1_end[1], 
                        l2_begin[1], 
                        l2_end[1]);
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
        const Vec2<T> outside = {{std::numeric_limits<T>::max(), point[1]}};
        size_t num_intersections = 0;
        auto prev = *begin;
        auto it = begin;
        it++;
        for (; it != end; it++) {
            bool intersects = line_intersect(
                                            point, 
                                            outside, 
                                            prev, 
                                            *it
                                        );
            num_intersections += (size_t)intersects;
            if (std::abs(cross(point - prev, *it - prev)) < EPS) {
                return PointLocation::Border;
            }
            prev = *it;
        }

        num_intersections += (size_t)line_intersect(
                                        point, 
                                        outside, 
                                        prev, 
                                        *begin
                                     );
        if (std::abs(cross(point - prev, *begin - prev)) < EPS) {
           return PointLocation::Border;
        }

        if (num_intersections % 2 == 1) {
            return PointLocation::Inside;
        } else {
            return PointLocation::Outside;
        }
    }
    
} // namespace popup
