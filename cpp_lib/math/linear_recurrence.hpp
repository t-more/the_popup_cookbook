// Author: Marcus Östling, Tomas Möre 2019
#pragma once
#include "matrix.hpp"
#include <vector>
#include <cassert>
#include <algorithm>

namespace popup {

/**
 *  Calculate the k'th linear recurrences given
 *  the N+1 first coefficients and N first
 *  values.
 *
 *  Complexity: (O(N^3 log(k)))
 *
 *  Inspired by:
 *  http://fusharblog.com/solving-linear-recurrence-for-programming-contest/
 */
class LinearRecurrence {
    Matrix<int64_t> init_matrix_;
    Matrix<int64_t> matrix_;
    std::vector<int64_t> coefficient_;
    std::vector<int64_t> init_values_;
    size_t degree_;
    size_t steps_ = 0;

    /**
     * Setup the matrix and inital values for
     * the linear recurrences.
     */
    void construct_init_matrix() {
        matrix_ = Matrix<int64_t>((int)(this->degree_+1));

        // Set 1's across the matrix diagonalish
        for (size_t c = 1, r = 0; c < this->degree_+1; c++, r++) {
                this->matrix_(r, c) = 1;
        }

        this->matrix_(this->degree_, this->degree_) = 1;

        for (size_t c = 0; c < this->degree_; c++) {
                this->matrix_(this->degree_-1, c) =
                     this->coefficient_[this->coefficient_.size()-1-c];
        }

        this->init_matrix_ = Matrix<int64_t>(this->matrix_);
        this->init_values_.push_back(this->coefficient_[0]);
    }

public:
    LinearRecurrence(
            size_t degree,
            std::vector<int64_t> &coefficient,
            std::vector<int64_t> &init_values
    ) {
        assert(degree+1 == coefficient.size());

        this->degree_ = degree;
        this->coefficient_ = coefficient;
        this->init_values_ = std::vector<int64_t>(init_values);

        construct_init_matrix();
    }

    /**
     *  Calculates the n'th term in the linear recurrence modulo moduli
     */
    int64_t get(size_t n, int64_t moduli) {
        size_t exponent = (degree_ >= n) ? 1 : n - degree_ + 1;
        Matrix<int64_t> tmp = this->matrix_.pow_modulus(exponent, moduli);
        std::vector<int64_t> iv_tmp(init_values_);
        for (auto &i : iv_tmp) {
            i = (i % moduli + moduli) % moduli;
        }
        std::vector<int64_t> res = mul_mod(tmp, iv_tmp, moduli);
        int64_t ans = res[res.size()-2];
        if (degree_ >= n) {
              ans = res[n-1];
        }

        return (ans % moduli + moduli) % moduli;
    }
};
} // namespace popup
