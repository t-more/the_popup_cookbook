#pragma once
#include <cmath>
#include <initializer_list>
#include <cstring>
#include <array>

#include <iostream>

#include "point.hpp"

namespace popup {

    template <unsigned int Dim, typename T>
    class Vec {
        Point<Dim, T> point_;
    public:

        Vec() {}

        Vec(const T e) : point_(Point<Dim,T>(e)) {}

        Vec(const T init[Dim]) : point_(Point<Dim,T>(init)) {}

        Vec(const std::array<T, Dim>& arr) : point_(arr){}

        Vec(const Vec<Dim, T>& other) : point_(other.point_) {}

        Vec(const Point<Dim, T>& point) : point_(point) {}

        size_t size() const {
            return point_.dim();
        }
        size_t dim() const {
            return point_.dim();
        }

        Point<Dim,T> as_point() const {
            return point_;
        }

        const Point<Dim,T>& point() const {
            return point_;
        }



        // Iterator defenitions

        inline T* begin() {
            return point_.begin();
        }

        inline T* end() {
            return point_.end();
        }

        const T *  begin() const {
            return point_.begin();
        }

        const  T * end() const {
            return point_.end();
        }


        inline const T * cbegin() const {
            return point_.cbegin();
        }

        inline const T * cend() const {
            return point_.cend();
        }

        //// Arithmetic operations

        // Addition
        inline void operator+=(const Vec<Dim, T>& other) {
            point_ += other.point_;
        }

        inline friend Vec<Dim, T> operator+(const Vec<Dim, T>& lhs, const Vec<Dim, T>& rhs) {
            return Vec(lhs.point_ + rhs.point_);
        }

        inline void operator+=(const T& s) {
            point_ += s;
        }

        inline friend Vec<Dim, T> operator+(const T& lhs, const Vec<Dim, T>& rhs) {
            return lhs + rhs.point_;
        }

        inline friend Vec<Dim, T> operator+(const Vec<Dim, T>& lhs, const T& rhs) {
            return lhs.point_ + rhs;
        }

        // Subtraction
        inline void operator-=(const Vec<Dim, T>& other) {
            point_ -= other.point_;
        }

        inline friend Vec<Dim, T> operator-(const Vec<Dim, T>& lhs, const Vec<Dim, T>& rhs) {
            return lhs.point_ - rhs.point_;
        }

        inline void operator-=(const T& s) {
            point_ -= s;
        }

        inline friend Vec<Dim, T> operator-(const T& lhs, const Vec<Dim, T>& rhs) {
            return lhs - rhs.point_;
        }

        inline friend Vec<Dim, T> operator-(const Vec<Dim, T>& lhs, const T& rhs) {
            return lhs.point_ - rhs;
        }

        // Multiplication
        inline void operator*=(const Vec<Dim, T>& other) {
            point_ *= other.point_;
        }

        inline friend Vec<Dim, T> operator*(const Vec<Dim, T>& lhs, const Vec<Dim, T>& rhs) {
            return lhs.point_ * rhs.point_;
        }

        inline void operator*=(const T& s) {
            point_ *= s;
        }

        inline friend Vec<Dim, T> operator*(const T& lhs, const Vec<Dim, T>& rhs) {
            return lhs * rhs.point_;
        }

        inline friend Vec<Dim, T> operator*(const Vec<Dim, T>& lhs, const T& rhs) {
            return lhs.point_ * rhs;
        }

        // Divison
        inline void operator/=(const Vec<Dim, T>& other) {
            point_ /= other.point_;
        }

        inline friend Vec<Dim, T> operator/(const Vec<Dim, T>& lhs, const Vec<Dim, T>& rhs) {
            return lhs.point_ / rhs.point_;
        }

        void operator/=(const T& s) {
            point_ /= s;
        }

        friend Vec<Dim, T> operator/(const T& lhs, const Vec<Dim, T>& rhs) {
            return lhs / rhs.point_;
        }

        friend Vec<Dim, T> operator/(const Vec<Dim, T>& lhs, const T& rhs) {
            return lhs.point_ / rhs;
        }

        // Arithmetic end
        inline T at(size_t i) const {
            return point_[i];
        }

        inline T operator[](size_t i) const {
            return point_[i];
        }

        inline T& operator[](size_t i) {
            return point_[i];
        }

        T dot(const Vec<Dim, T>& other) const {
            T res = T();
            auto it1 = cbegin();
            auto it2 = other.cbegin();
            while (it1 != cend()) {
                res += (*(it1++)) * (*(it2++));
            }
            return res;
        }

        T norm() const {
            return std::sqrt(norm_square());
        }

        T norm_square() const {
            T res = T();
            for (const auto &  e  : *this) {
                res += e * e;
            }
            return res;
        }

        /**
         * Mutates the vector into a normalized form.
         */
        void normalize() {
            point_ /= norm();
        }

        /**
         * Mutates the vector into a normalized form.
         */
        Vec<Dim, T> normalized() const {
            return (*this) / norm();
        }

        bool comparable(const Vec<Dim, T>& other, const T epsilon = 1e-9) const {
            return point_.comparable(other.point_, epsilon);
        }

        Vec<Dim, T> projected_on(const Vec<Dim, T>& other) {
            return dot(other) / other.norm_square();
        }

        T scalar_projection_on(const Vec<Dim, T>& other) {
            return dot(other.normalized());
        }

    };

    template<typename T>
    using Vec2 = Vec<2, T>;

    template<typename T>
    using Vec3 = Vec<3, T>;


    template<typename T>
    Vec<3, T> cross(const Vec<3, T>& a,  const Vec<3, T>& b) {
        return {
                a[1] * b[2] - a[2] * b[1],
                a[2] * b[0] - a[0] * b[2],
                a[0] * b[1] - a[1] * b[0]
        };
    }

    template<typename T>
    T cross(const Vec<2, T>& a,  const Vec<2, T>& b) {
        return a[0] * b[1] - a[1] * b[0];
    }

    // Returns the anti clocwise angle between two vectors
    template<typename T>
    double angle(const Vec<2, T>& a,  const Vec<2, T>& b) {
        auto cos_angle = a.dot(b) / (a.norm() * b.norm());
        auto cross_sign = cross(a, b);
        auto res = std::acos(cos_angle);
        if (cross_sign < 0) {
            res += PI;
        }
        return res;
    }


    template<typename T>
    Vec2<T> vec2(const T p1, const T p2){
        return {{p1,p2}};
    }

    template <unsigned int Dim, typename T>
    std::ostream& operator<<(std::ostream& out, const Vec<Dim, T> &vec) {
        return out << vec.point_;
    }

    template <unsigned int Comp, unsigned int Dim, typename T>
    Vec<Dim,T>& max_dim(Vec<Dim,T>& a, Vec<Dim,T>& b) {
        return max_dim<Comp>(a.point_, b.point_);
    }

    template <unsigned int Comp, unsigned int Dim, typename T>
    Vec<Dim,T>& min_dim(Vec<Dim,T>& a, Vec<Dim,T>& b) {
        return min_dim<Comp>(a.point_, b.point_);
    }
} // namespace popup
