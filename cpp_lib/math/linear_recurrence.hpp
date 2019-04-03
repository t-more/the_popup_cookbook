#pragma once
#include "matrix.hpp"
#include <vector>
#include <cassert>
#include <algorithm>

template <typename T>
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
                this->matrix_(c, r) = 1;
        }

        this->matrix_(this->degree_, this->degree_) = 1;

        for (int c = 0; c < this->degree_; c++) {
                this->matrix_(c, this->degree_-1) = 
                    this->coefficient_[this->coefficient_.size()-1-c];
        }

        this->init_matrix_ = Matrix<int64_t>(this->matrix_);
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
        std::reverse(this->init_values_.begin(), this->init_values_.end());

        construct_init_matrix();
    }

    void step(size_t n) {

    }

    int64_t get(size_t n) {
        return 0;
    }
};
