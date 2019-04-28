#pragma once
#include "./vec.hpp"

#include <cassert>
#include <utility>
#include <iostream>

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
            sum += (last[0] - current[0]) * (last[1] * current[1]) / 2.0;
            std::cerr << current[0] << " " << first[1] << std::endl;
            last = current;
        }
        std::cerr << first[0] << " " << first[1] << std::endl;
        sum += (last[0] - first[0]) * (last[1] * first[1]) / 2.0;

        return {sum < 0 ? ClockOrder::CCW : ClockOrder::CW
                , std::abs(sum)};
    }
} // namespace popup
