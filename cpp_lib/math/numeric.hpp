#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdint>

class SieveCache {
    std::vector<bool> is_prime_;
    uint64_t max_number_;
    uint64_t primes_found_ ;
public:

    SieveCache(uint64_t max_number) {
        max_number_ = max_number;
        primes_found_ = 1; // 2 is prime and not counted
        const auto max_index = (max_number+1) / 2;
        is_prime_ = std::vector<bool>(max_index, true);

        for (uint64_t i = 1;  i < max_index; i++) {
            const uint64_t number = 2 * i + 1;
            if (is_prime_[i]) {
                primes_found_ += 1;
                for (uint64_t n = 3 * number ; n < max_number; n = n + 2 * number) {
                    size_t n_idx = (n - 1)/2;
                    //std::cerr << "Not prime: " << n << " idx: " << n_idx << std::endl;

                    is_prime_[n_idx] = false;
                }
            }
        }
    }

    uint64_t primes_found() const {
        return primes_found_;
    }

    bool is_prime(uint64_t number) const {
        if (number < 2) {
            return false;
        } else if (number == 2) {
            return true;
        } else if (number % 2 == 0) {
            return false;
        } else {
            return is_prime_[(number - 1) / 2];
        }
    }
};
