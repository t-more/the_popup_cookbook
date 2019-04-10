// Author: Marcus Östling, Tomas Möre
#pragma once
#include <tuple>
#include <optional>

namespace popup {

    template <typename T, typename N>
    std::tuple<T, N, N> extended_euclidean(T a, T b) {
        if(a == 0) {
            return {b, 0, 1};
        }

        auto [gcd, x, y] = extended_euclidean<T, N>(b % a, a);

        return {gcd, y - (b/a) * x, x};
    }

    template <typename T, typename N>
    std::optional<T> mod_inverse(T a, T m) {
        auto [gcd, x, y] = extended_euclidean<T, N>(a, m);

        if(gcd != 1) {
            return std::nullopt;
        }

        return (x % (N)m + (N)m) % m;
    }

    template <typename T>
    T mul_mod(T a, T b, T m) {
        return (a*b)%m;
    }

    template <typename T, typename N>
    std::optional<T> div_mod(T a, T b, T m) {
        a %= m;
        auto inv = mod_inverse<T, N>(b, m);
        if(inv.has_value()) {
            return (inv.value() * a) % m;
        }
        return inv;
    }

    template <typename T>
    T add_mod(T a, T b, T m) {
        return (a+b)%m;
    }

    template <typename T>
    T sub_mod(T a, T b, T m) {
        a %= m;
        b %= m;
        if(b > a) a += m;
        return (a-b)%m;
    }

    /**
     *  Chinese rest theorem where m1 and m2 are co-prime.
     */
    template<typename T>
    std::optional<T> chinese_rest_theorem(T a1, T m1, T a2, T m2) {
        auto [gcd, n1, n2] = extended_euclidean<T, int64_t>(m1, m2);
        if (gcd == 1) {
            // Specia case of two acording to wikipedia
            T ans = ((a1 * (n2 + m1)) % m1) * m2 + ((a2 * (n1 + m2)) % m2) * m1;
            if (ans >= m1 * m2)
                ans -= m1 * m2;
            return ans;
        }
        return std::nullopt;
    }

    /**
     *  General case of chinese rest theorem where
     *  m1 and m2 doesn't have to be co-prime.
     *  https://en.wikipedia.org/wiki/Chinese_remainder_theorem
     */
    template<typename T>
    std::optional<T> general_chinese_rest_theorem(T a1, T m1, T a2, T m2) {
        auto [gcd, n1, n2] = extended_euclidean<T, int64_t>(m1, m2);
        if(((a1 + gcd) % gcd) == (a2 + gcd) % gcd) { // unique solution
            T lcm = (m1*m2)/gcd;
            T ans = ((a1 * (n2 + m1)) % m1) * m2 + ((a2 * (n1 + m2)) % m2) * m1;
            return (ans/gcd) % lcm;
        }
        return std::nullopt;
    }

} // namespace popup
