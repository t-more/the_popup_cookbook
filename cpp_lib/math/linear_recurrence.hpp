#pragma once
#include "matrix.hpp"
#include <vector>
#include <cassert>
#include <algorithm>

namespace popup {

class LinearRecurrence {
    Matrix<int64_t> init_matrix_;
    Matrix<int64_t> matrix_;
    std::vector<int64_t> coefficient_;
    std::vector<int64_t> init_values_;
    size_t degree_;
    size_t steps_ = 0;
    
    void construct_init_matrix() {
        matrix_ = Matrix<int64_t>(this->degree_+1);

        // Set 1's across the matrix diagonalish
        for (int c = 1, r = 0; c < this->degree_+1; c++, r++) {
                this->matrix_(r, c) = 1;
        }

        this->matrix_(this->degree_, this->degree_) = 1;

        for (int c = 0; c < this->degree_; c++) {
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
        //std::reverse(this->init_values_.begin(), this->init_values_.end());

        construct_init_matrix();
    }

    void step(size_t n) {
    }

    int64_t get(size_t n, int64_t moduli) {
        //this->matrix_.modulus(moduli);
        Matrix<int64_t> tmp = this->matrix_.pow_modulus(n, moduli);
        for (auto i : init_values_) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        std::vector<int64_t> iv_tmp(init_values_);
        for (auto &i : iv_tmp) {
            i = (i % moduli + moduli) % moduli;
        }
        std::vector<int64_t> res = mul_mod(tmp, iv_tmp, moduli);
        for(auto i : res) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        int64_t ans = res[res.size()-2];
        return (ans % moduli + moduli) % moduli;
    }
};
} // namespace popup
