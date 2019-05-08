// Author: Marcus Östling, Tomas Möre
#pragma once
#include <vector>

namespace popup {

    /**
     * "real" modulus
     */
    unsigned long long mod(unsigned long long x, unsigned long long m) {
        return ((x % m) + m) % m;
    }

    /**
     *  Multiply modulus m using magic floating point properties
     *  (big endian manta)
     *
     *  source: wikipedia
     */
    uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m)
    {
        long double x;
        uint64_t c;
        int64_t r;
        if (a >= m) a %= m;
        if (b >= m) b %= m;
        x = a;
        c = x * b / m;
        r = (int64_t)(a * b - c * m) % (int64_t)m;
        return r < 0 ? r + m : r;
    }

    /**
     * A rolling hash with constant look up time.
     *
     * O(n), hashing
     * O(1), look up
     */
    class RollingHash {
        const uint64_t m = 922337220451ULL;                                    
        const uint64_t p = 16069ULL;                                           
        const uint64_t inv = 184249329619ULL;
        std::vector<uint64_t> inverses; // pow(2, 31) % 1e9+9
        std::vector<uint64_t> hashes;
        std::string str;

        void compute_hash(){
            uint64_t hash_value = 0;
            uint64_t p_pow = 1;
            for (char c : str) {
                hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
                hashes.push_back(hash_value);
                p_pow = p_pow *p % m ;
            }
        }

        void compute_inverses() {
            inverses.resize(str.size());
            inverses[0] = 1;
            inverses[1] = inv;
            for(uint64_t i = 2; i < str.size(); i++) {
                inverses[i] = mul_mod(inverses[i-1], inverses[1], m);
            }     
        }

    public:
        RollingHash(std::string &str) {
            this->str = str;
            compute_inverses();
            compute_hash();
        }

        /** 
         *  Return complete hash
         *
         *  O(1)
         */
        uint64_t hash() {
            return subhash(0, hashes.size());
        }

        /**
         *  Return subhash between i and j
         *
         * O(1)
         */
        uint64_t subhash(size_t i, size_t j) {
            j--;
            if (i == 0) {
                return hashes[j];
            } else {
                i--;
                return mul_mod((hashes[j] + m - hashes[i])%m, inverses[i+1], m);
            }
        }

    };

}
