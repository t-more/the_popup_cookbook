// Author Marcus Östling, Tomas Möre
#pragma once
#include "vec.hpp"
#include "point.hpp"
#include "line_segment.hpp"

#include <cassert>
#include <utility>
#include <iostream>
#include <limits>

namespace popup {
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

    /**
     *  Given iterators begin and end to a container of points,
     *  calculate the area of a simple polygon and whether
     *  the polygon is given clockwise or counter-clockwise.
     *
     *  O(n)
     */
    template<typename T, typename RAItr>
    std::pair<ClockOrder, T> polygon_order_area(RAItr begin, RAItr end) {
        assert((end - begin) >= 3);

        T sum = T();
        auto itr = begin;
        Vec2<T> first = *begin;
        Vec2<T> prev = *begin;
        itr++;

        while (itr != end) {
            Vec2<T> current = *(itr++);
            sum += cross(current - first, prev - first);
            prev = current;
        }
        sum /= 2.0;

        return {
            sum < 0 ? ClockOrder::CCW : ClockOrder::CW,
            std::abs(sum)
        };
    }

    enum PointLocation {
        Outside = 0,
        Border,
        Inside
    };

    /**
     *  Given iterators begin and end to a container of points,
     *  check whether a point is inside, outside or on the border
     *  of a simple polygon using winding numbers.
     *
     *  O(n)
     */
    template<typename T, typename RAItr>
    PointLocation point_in_polygon(const Point2<T>& point, RAItr begin, RAItr end) {
        const double EPS = 1e-9;
        double angle_sum = 0;
        auto prev = *begin;
        auto it = begin;
        it++;
        for (; it != end; it++) {
            Vec2<T> v1 = Vec2<T>(*it - point).normalized();
            Vec2<T> v2 = Vec2<T>(prev - point).normalized();
            double cross_angle = cross(v1, v2);
            double dot_angle = v1.dot(v2);
            angle_sum += std::atan2(cross_angle, dot_angle);
            if (LineSegment<T>(prev, *it).contains_point(point)) {
                return PointLocation::Border;
            }
            prev = *it;
        }

        Vec2<T> v1 = Vec2<T>(*begin - point).normalized();
        Vec2<T> v2 = Vec2<T>(prev - point).normalized();
        double cross_angle = cross(v1, v2);
        double dot_angle = v1.dot(v2);
        angle_sum += std::atan2(cross_angle, dot_angle);
        if (LineSegment<T>(prev, *begin).contains_point(point)) {
            return PointLocation::Border;
        }

        if (std::abs(angle_sum) <= EPS) {
            return PointLocation::Outside;
        } else {
            return PointLocation::Inside;
        }
    }
} // namespace popup
