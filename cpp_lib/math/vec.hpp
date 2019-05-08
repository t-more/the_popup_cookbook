// Author: Tomas Möre, Markus Östling
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

        size_t dim() const {
            return point_.dim();
        }
        /**
         * Redefiniton of dim, to conform to standard c++ naming.
         */
        size_t size() const {
            return point_.dim();
        }

        /**
         * Gives a copy of internal point used in the vector
         */
        Point<Dim,T> as_point() const {
            return point_;
        }

        /**
         * Gives the internal point used in the vector as a reference (can be
         * good for large points)
         */
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

        /**
         * The operator methods defined bellow contains elementwise mathematical
         * operation for points.
         */

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

        /**
         * Operator free accessor. Non safe for bounds
         */
        inline T at(size_t i) const {
            return point_[i];
        }

        inline T operator[](size_t i) const {
            return point_[i];
        }

        inline T& operator[](size_t i) {
            return point_[i];
        }

        /**
         * Dot product between two vectors
         *
         * O(n)
         */
        T dot(const Vec<Dim, T>& other) const {
            T res = T();
            auto it1 = cbegin();
            auto it2 = other.cbegin();
            while (it1 != cend()) {
                res += (*(it1++)) * (*(it2++));
            }
            return res;
        }

        /**
         * Gives the length of the vector. Uses square root and thefore should
         * not be used with non floating point values.
         *
         * O(n)
         */
        T norm() const {
            return std::sqrt(norm_square());
        }

        /**
         * Gives the squared length of a vector. Good for simple
         * comparisons. Between vectors.
         *
         * O(n)
         */
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
         * Returns a new vector that is the normalized version of the current
         * vector
         */
        Vec<Dim, T> normalized() const {
            return (*this) / norm();
        }

        /**
         * Checks wther two points are comparable, for floating point values
         * ther also exists an epsilon check for error handling reasons. It is
         * expected that the optional argument is rounded down to 0 for other
         * kinds of values.
         *
         * O(n)
         */
        bool comparable(const Vec<Dim, T>& other, const T epsilon = 1e-9) const {
            return point_.comparable(other.point_, epsilon);
        }

        /**
         * Returns a vector that is the projection of this vector uppon some
         * other vector.
         *
         * O(n)
         */
        Vec<Dim, T> projected_on(const Vec<Dim, T>& other) {
            return dot(other) / other.norm_square();
        }

        /**
         * Returns the fraction of how much of the current vector is projected
         * unto another vector.
         *
         * O(n)
         */
        T scalar_projection_on(const Vec<Dim, T>& other) {
            return dot(other.normalized());
        }

    };

    template<typename T>
    using Vec2 = Vec<2, T>;

    template<typename T>
    using Vec3 = Vec<3, T>;


    /**
     * Returns the cross product of a 3-d vector
     *
     * O(1)
     */
    template<typename T>
    Vec<3, T> cross(const Vec<3, T>& a,  const Vec<3, T>& b) {
        return {
                a[1] * b[2] - a[2] * b[1],
                a[2] * b[0] - a[0] * b[2],
                a[0] * b[1] - a[1] * b[0]
        };
    }

    /**
     * Returns the cross product of a 1-d vector
     *
     * O(1)
     */
    template<typename T>
    T cross(const Vec<2, T>& a,  const Vec<2, T>& b) {
        return a[0] * b[1] - a[1] * b[0];
    }


    /**
     * Returns the anti clockwise angle between two 2d vectors
     *
     * O(1)
     */
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

    /**
     * Utility function for creating two dimentional vectors
     *
     * O(1)
     */
    template<typename T>
    inline Vec2<T> vec2(const T& p1, const T& p2){
        return {{p1,p2}};
    }

    /**
     * Prints the vector as a json-like array.
     *
     * O(1)
     */
    template <unsigned int Dim, typename T>
    std::ostream& operator<<(std::ostream& out, const Vec<Dim, T> &vec) {
        return out << vec.point_;
    }

    /**
     * Returns the element with the largest value at certain dimension given by
     * the template argument Comp
     */
    template <unsigned int Comp, unsigned int Dim, typename T>
    inline Vec<Dim,T>& max_dim(const Vec<Dim,T>& a, const Vec<Dim,T>& b) {
        return max_dim<Comp>(a.point_, b.point_);
    }

    /**
     * Returns the element with the smallest value at certain dimension given by
     * the template argument Comp
     */
    template <unsigned int Comp, unsigned int Dim, typename T>
    inline Vec<Dim,T>& min_dim(Vec<Dim,T>& a, Vec<Dim,T>& b) {
        return min_dim<Comp>(a.point_, b.point_);
    }
} // namespace popup
