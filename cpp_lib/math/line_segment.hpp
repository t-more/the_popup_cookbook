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

    template <typename T>
    class LineSegment {
        Point<2, T> start_;
        Point<2, T> end_;

        // Contains info about with of start and end has the largest x value.
        Point<2, T>* min_;
        Point<2, T>* max_;
        /**
         * This method assumes that this object and the other passed as argument
         * are colinear. Will return a line segment containting the overlap
         */
        std::optional<LineSegment> interval_overlap(
                const LineSegment& other,
                T eps = 1e-9) const {
            auto first = std::max(*min_, *other.min_);
            auto second = std::min(*max_, *other.max_);
            if (first[0] <= second[0] + eps) {
                return {{first, second}};
            } else {
                return std::nullopt;
            }
        };

    public:
        LineSegment() {
            min_ = &start_;
            max_ = &end_;
        }
        LineSegment(const Point<2, T>& a, const Point<2, T>& b) {
            start_ = a;
            end_ = b;

            min_ = &end_;
            max_ = &start_;
            for (size_t i = 0; i < 2; i++) {
                if (a[i] < b[i]) {
                    min_ = &start_;
                    max_ = &end_;
                    break;
                }
            }
        }

        Point<2, T> start() {
            return start_;
        }

        Point<2, T> end() {
            return end_;
        }
        /**
         * Return the length of the line segment.
         */
        T length() {
            return start_.distance(end_);
        }


        bool contains_point(const Point<2, T>& point) const {
            const double EPS = 1e-9;
            double len1 = point.distance_to(*min_);
            double len2 = point.distance_to(*max_);
            double total_len = (*min_).distance_to(*max_);
            return (std::abs(len1+len2-total_len) < EPS);
        }


        /**
         *
         */
        bool intersects(const LineSegment &other) const {
            const auto interval_intersects_1d =
                [](T ae, T be, T ce, T de) {
                    if (ae > be) std::swap(ae, be);
                    if (ce > de) std::swap(ce, de);
                    return std::max(ae, ce) <= std::min(be, de);
                };

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
        bool colinear(const LineSegment &other) const {
            auto v1 = Vec<2, T>(*min_ - *max_);
            v1.normalize();
            auto v2 = Vec<2, T>(*other.min_ - *other.max_);
            v2.normalize();
            Vec<2, T> v3 = Vec<2, T>(*min_ - *other.max_);
            if (min_->comparable(*other.max_)) {
                v3 = Vec<2, T>(*max_ - *other.min_);
            }
            v3.normalize();
            return v1.comparable(v2) && v3.comparable(v1);
        }

        /**
         * Returns true if the two line segments are colinear.
         */
        bool parallel(const LineSegment &other) const {
            auto v1 = Vec<2, T>(*min_ - *max_);
            v1.normalize();
            auto v2 = Vec<2, T>(*other.min_ - *other.max_);
            v2.normalize();
            return v1.comparable(v2);
        }


        /**
         *
         */
        std::pair<IntersectionType, LineSegment> intersection(const LineSegment &other, double eps = 1e-9) const {
            if (colinear(other)) {
                auto opt_overlap = interval_overlap(other);
                if (opt_overlap.has_value()) {
                    auto& overlap = opt_overlap.value();
                    if (overlap.start_.comparable(overlap.end_)) {
                        return {IntersectionType::PointIntersect
                                , overlap};
                    } else {
                        return {IntersectionType::SegmentIntersect
                                , overlap};
                    }
                }
            } else if (intersects(other)) {
                if (start_.comparable(end_)) {
                    return {IntersectionType::PointIntersect
                            , *this};
                } else if (other.start_.comparable(other.end_)) {
                    return {IntersectionType::PointIntersect
                            , other};
                }
                // auto k1 = (start_[1] - end_[1]) / (start_[0] - end_[0]);
                // if (std::abs(start_[0] - end_[0]) < eps) {
                //     k1 = 0.0;
                // }
                // auto k2 = (other.start_[1] - other.end_[1]) / (other.start_[0] - other.end_[0]);
                // if (std::abs(other.start_[0] - other.end_[0]) < eps) {
                //     k2 = 0.0;
                // }
                // auto m1 = end_[1] - end_[0] * k1;
                // auto m2 = other.end_[1] - other.end_[0] * k2;
                // auto x = (m2 - m1) / (k1 - k2);
                // if (std::abs(k1 - k2) < eps) {
                //     x = 0.0;
                // }
                // auto y = k1 * x + m1;

                auto t = ((start_[0] - other.start_[0]) * (other.start_[1] - other.end_[1]) - (start_[1] - other.start_[1]) * (other.start_[0] - other.end_[0]))
                    / ((start_[0] - end_[0])*(other.start_[1]-other.end_[1]) - (start_[1] - end_[1]) * (other.start_[0] - other.end_[0]));
                //auto u = -((start_[0] - end_[0])*(start_[1] - other.start_[1]) - (start_[1] - end_[1]) * (start_[0] - other.start_[0])) / ((start_[0] - end_[0]) * (other.start_[1] - other.end_[1]) - (start_[1] - end_[1]) * (other.start_[0] - other.end_[0]));
                Point<2,T> point = {{ start_[0] + t * (end_[0] - start_[0])
                                      , start_[1] + t * (end_[1] - start_[1])}};
                if (contains_point(point)) {
                    return {IntersectionType::PointIntersect
                            , LineSegment(point, point)};
                }
            }
            return {IntersectionType::None
                    , LineSegment<T>()};

        };

    };

}
