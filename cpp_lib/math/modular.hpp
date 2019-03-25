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

} // namespace popup
