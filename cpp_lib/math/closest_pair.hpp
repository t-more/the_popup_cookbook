#pragma once

#include <utility>
#include <vector>
#include <limits>
#include "vec.hpp"


namespace popup {

    // template<typename T, typename RAItr>
    // void cp_update(
    //     const Point2<T>& a, const Point2<T>& b,
    //     T& min_dist,
    //     std::pair<Point2<T>, Point2<T>>& min_points
    // ) {
    //     if (a.square_distance_to(b) < min_dist) {
    //         min_dist = a.square_distance_to(b);
    //         min_points = std::make_pair(a,b);
    //     }
    // }

    template<typename T, typename RAItr>
    void cp_rec(
        RAItr begin, RAItr end,
        std::vector<Point2<T>>& merge_buf,
        T& min_dist,
        std::pair<Point2<T>, Point2<T>>& min_points
    ) {

        const auto compare_y =
            [](const Point2<T>& a, const Point2<T>& b) {
                return a[1] < b[1];
            };
        const auto cp_update =
            [](const Point2<T>& a, const Point2<T>& b,
               T& min_dist,
               std::pair<Point2<T>, Point2<T>>& min_points ) {
                if (a.square_distance_to(b) < min_dist) {
                    min_dist = a.square_distance_to(b);
                    min_points = std::make_pair(a,b);
                }
            };

        if (end - begin <= 3) {
            for (auto o_itr = begin; o_itr != end; o_itr++) {
                for (auto i_itr = o_itr+1; i_itr != end; i_itr++) {
                    cp_update(*o_itr, *i_itr, min_dist, min_points);
                }
            }
            sort(begin, end, compare_y);
            return;
        }

        auto mid = begin + (end - begin) / 2;
        T mid_x = mid->at(0);

        cp_rec<T>(begin, mid, merge_buf, min_dist, min_points);
        cp_rec<T>(mid,   end, merge_buf, min_dist, min_points);

        merge(begin, mid, mid, end, merge_buf.begin(), compare_y);
        copy(merge_buf.begin(), merge_buf.begin() + (end - begin), begin);

        // Here merge_buf is used to store the point that we actually want to
        // look at
        auto c_points = merge_buf.begin();
        for (auto p_itr = begin; p_itr != end; p_itr++) {
            if (((p_itr->at(0) - mid_x) * (p_itr->at(0) - mid_x)) < min_dist) {
                for (auto cp_itr = c_points -1;
                     cp_itr != merge_buf.begin() -1
                         && (p_itr->at(1) - cp_itr->at(1)) * (p_itr->at(1) - cp_itr->at(1)) < min_dist;
                     --cp_itr
                ) {
                    cp_update(*p_itr, *cp_itr, min_dist, min_points);
                }
                *c_points = *p_itr;
                c_points++;
            }
        }
    }

    template<typename T, typename RAItr>
    std::pair<Point2<T>, Point2<T>> closest_pair(
        RAItr begin,
        RAItr end,
        T max_val = std::numeric_limits<T>::max()
    ) {
        sort(begin, end);
        std::vector<Point2<T>> merge_buffer(end - begin);
        T& min_dist = max_val;
        std::pair<Point2<T>,Point2<T>> min_points;

        cp_rec<T>(
            begin, end,
            merge_buffer,
            min_dist,
            min_points
        );
        return min_points;
    };
}
