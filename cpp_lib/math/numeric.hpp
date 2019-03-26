#pragma once

#include <cmath>
#include <vector>
#include <utility>



class SieveCache {
    std::vector<bool> is_prime_;
public:

    SieveCache(uint64_t max_number) {
        is_prime = std::vector<bool>(true);
        const auto max_num = std::sqrt(max_number);

        for (uint64_t i = 2;  (2 * i + 1) < max_num; i++) {
            const uint64_t number = 2 * i + 1;
            if (is_prime_[i]) {
                for (uint64_t n = 3 * number ; n < max_num; n = n + 2 * number) {
                    size_t n_idx = (n - 1)/2;
                    is_prime_[n_idx] = false;
                }
            }
        }
    }


    bool is_prime(uint64_t )



}
