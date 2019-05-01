#pragma once
#include "./vec.hpp"

#include <cassert>
#include <utility>
#include <iostream>
#include <limits>
#include <optional>

namespace popup {
    const double EPS = 1e-9;
    enum IntersectionType {
                           None = 0,
                           PointIntersect,
                           SegmentIntersect
    };

    template <unsigned int Dim, typename T>
    class LineSegment {
        Point<Dim, T> start_;
        Point<Dim, T> end_;

        // Contains info about with of start and end has the largest x value.
        Point<Dim, T>& min_x_;
        Point<Dim, T>& max_x_;
        /**
         * This method assumes that this object and the other passed as argument
         * are colinear. Will return a line segment containting the overlap
         */
        std::optional<LineSegment> interval_overlap(const LineSegment& other) {
            auto first = max_dim<0>(min_x_, other.min_x_);
            auto second = min_dim<0>(max_x_, other.max_x_);
            if (first[0] < second[0]) {
                return {first, second};
            } else {
                return std::nullopt;
            }
        };

    public:

        LineSegment(const Point<Dim, T>& a, const Point<Dim, T>& b) {
            start_ = a;
            end_ = b;
            if (a[0] < b[0]) {
                min_x_ = start_;
                max_x_ = end_;
            } else {
                max_x_ = start_;
                min_x_ = end_;
            }
        }

        /**
         * Return the length of the line segment.
         */
        T length() {
            return start_.distance(end_);
        }

        /**
         *
         */
        bool intersects(const LineSegment &other) {
            for (size_t i = 0; i < 2; i++) {
                auto insertects_in_dim = interval_intersects_1d(
                    start_[i],
                    end_[i],
                    other.start_[i],
                    other.end_[i]);
                if (!insertects_in_dim) return false;
            }
            return true;
        };

        /**
         * Returns true if the two line segments are colinear.
         */
        bool colinear(const LineSegment &other) {
            return std::abs((other.start_ - start_).det(end_ - start_)) < EPS
                && std::abs((other.end_  - start_).det(end_ - start_)) < EPS;
        }

        /**
         *
         */
        std::pair<IntersectionType, LineSegment> intersection(const LineSegment &other) {
            const auto interval_intersects_1d =
                [](T ae, T be, T ce, T de) {
                    if (ae > be) std::swap(ae, be);
                    if (ce > de) std::swap(ce, de);
                    return std::max(ae, ce) <= std::min(be, de);
                };
            if (colinear(other)) {
                auto opt_overlap = interval_overlap(other);
                if (opt_overlap->has_value()) {
                    auto& overlap = opt_overlap.value();
                    if (overlap.start_.comoparable(overlap.end_)) {
                        return {IntersectionType::PointIntersect
                                , overlap};
                    } else {
                        return {IntersectionType::SegmentIntersect
                                , overlap};
                    }
                }
            }

            for (size_t i = 0; i < 2; i++) {
                auto insertects_in_dim = interval_intersects_1d(
                    start_[i],
                    end_[i],
                    other.start_[i],
                    other.end_[i]);
                if (!insertects_in_dim) return false;
            }
            return true;
        };




    };

}
