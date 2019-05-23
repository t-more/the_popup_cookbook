// Author: Tomas Möre, Markus Östling
#pragma once
#include <cmath>
#include <initializer_list>
#include <cstring>
#include <array>
#include <iostream>

namespace popup {
    const double PI = 3.1415926535897932384626433;

    /**
     * A class for points of arbitrary dimentions.
     */
    template <unsigned int Dim, typename T>
    class Point {
        T arr_[Dim];
    public:

        Point() {
            std::fill(arr_, arr_+Dim, 0);
        }
        Point(const T e) {
            std::fill(arr_, arr_+Dim, e);
        }

        Point(const T init[Dim]) {
            arr_ = init;
        }

        Point(const Point<Dim, T>& other) {
            std::memcpy(arr_, other.arr_, sizeof(arr_));
        }

        Point(const std::array<T, Dim>& other) {
            std::memcpy(arr_, other.begin(), sizeof(arr_));
        }

        size_t dim() const {
            return Dim;
        }

        /**
         * Redefiniton of dim, to conform to standard c++ naming.
         */
        size_t size() const {
            return Dim;
        }

        T* begin() {
            return arr_;
        }

        T* end() {
            return arr_ + size();
        }

        const T * begin() const {
            return arr_;
        }

        const T * end() const {
            return arr_ + size();
        }

        const T * cbegin() const {
            return arr_;
        }

        const T * cend() const {
            return arr_ + size();
        }

        /**
         * Operator free accessor. Non safe for bounds
         */
        T at(const size_t i) const {
            return arr_[i];
        }

        /**
         * Operator free accessor. Non safe for bounds
         */
        T& at(const size_t i) {
            return arr_[i];
        }

        T operator[](const size_t i) const {
            return at(i);
        }

        T& operator[](const size_t i) {
            return at(i);
        }

        /**
         * The operator methods defined bellow contains elementwise mathematical
         * operation for points.
         */

        // Addition
        void operator+=(const Point<Dim, T>& other) {
            auto it = other.cbegin();
            for (auto& e : *this) {
                e = *(it++) + e;
            }
        }

        friend Point<Dim, T> operator+(const Point<Dim, T>& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(lhs);
            res += rhs;
            return res;
        }

        void operator+=(const T& s) {
            for (auto& e : *this) {
                e = s + e;
            }
        }

        friend Point<Dim, T> operator+(const T& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(rhs);
            res += lhs;
            return res;
        }

        friend Point<Dim, T> operator+(const Point<Dim, T>& lhs, const T& rhs) {
            auto res = Point(lhs);
            res += rhs;
            return res;
        }

        // Subtraction
        void operator-=(const Point<Dim, T>& other) {
            auto it = other.cbegin();
            for (auto& e : *this) {
                e = e - *(it++);
            }
        }

        friend Point<Dim, T> operator-(const Point<Dim, T>& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(lhs);
            res -= rhs;
            return res;
        }

        void operator-=(const T& s) {
            for (auto& e : *this) {
                e = e - s;
            }
        }

        friend Point<Dim, T> operator-(const T& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(lhs);
            res -= rhs;
            return res;
        }

        friend Point<Dim, T> operator-(const Point<Dim, T>& lhs, const T& rhs) {
            auto res = Point(lhs);
            res -= rhs;
            return res;
        }

        // Multiplication
        void operator*=(const Point<Dim, T>& other) {
            auto it = other.cbegin();
            for (auto& e : *this) {
                e = *(it++) * e;
            }
        }

        friend Point<Dim, T> operator*(const Point<Dim, T>& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(lhs);
            res *= rhs;
            return res;
        }

        void operator*=(const T& s) {
            for (auto& e : *this) {
                e = e / s;
            }
        }

        friend Point<Dim, T> operator*(const T& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(lhs);
            res *= rhs;
            return res;
        }

        friend Point<Dim, T> operator*(const Point<Dim, T>& lhs, const T& rhs) {
            auto res = Point(lhs);
            res /= rhs;
            return res;
        }

        // Divison
        void operator/=(const Point<Dim, T>& other) {
            auto it = other.cbegin();
            for (auto& e : *this) {
                e = *(it++) / e;
            }
        }

        friend Point<Dim, T> operator/(const Point<Dim, T>& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(lhs);
            res /= rhs;
            return res;
        }

        void operator/=(const T& s) {
            for (auto& e : *this) {
                e = e / s;
            }
        }

        friend Point<Dim, T> operator/(const T& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(lhs);
            res /= rhs;
            return res;
        }

        friend Point<Dim, T> operator/(const Point<Dim, T>& lhs, const T& rhs) {
            auto res = Point(lhs);
            res /= rhs;
            return res;
        }


        /**
         * Gives the square distance between two poitns. Usefull for
         * quantifying the distance between non floating point values.
         */
        T square_distance_to(const Point<Dim, T>& other) const {
            T res = T();
            auto it1 = cbegin();
            auto it2 = other.cbegin();

            while (it1 != cend()) {
                auto temp = (*it1++) - (*it2++);
                res += temp * temp;
            }
            return res;
        }

        /**
         * Gives the distance between two poitns. Warning, this method is
         * undefined for non floating point types.
         */
        T distance_to(const Point<Dim, T>& other) const {
            return std::sqrt(square_distance_to(other));
        }

        /**
         * Checks wther two points are comparable, for floating point values
         * ther also exists an epsilon check for error handling reasons. It is
         * expected that the optional argument is rounded down to 0 for other
         * kinds of values.
         *
         * O(n)
         */
        bool comparable(const Point<Dim, T>& other, const T epsilon = 1e-9) const {
            for (size_t i = 0; i < Dim; i++) {
                if (!(std::abs((*this)[i] - other[i]) < epsilon)) {
                    return false;
                }
            }
            return true;
        }

        /**
         * Elementwise exact equality. Warning, unsafe with floating point
         * values.
         *
         * O(n)
         */
        bool operator==(const Point<Dim, T>& other) {
            for (size_t i = 0; i < dim(); i++) {
                if (at(i) != other.at(i)) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Point<Dim, T>& other) {
            return !(*this == other);
        }

        /**
         * Default comparison operator for points. Primarily sorts by 'low
         * dimentions'. Only compares higher dimentions if the values of the
         * lower are equal.
         *
         * EG (1,3) < (2,5) and (2,5) < (2,3)
         *
         * O(n)
         */
        bool operator<(const Point<Dim, T>& other) const {
            for (size_t i = 0; i < Dim; i++) {
                if (at(i) < other.at(i)) {
                    return true;
                } else if (at(i) != other.at(i)) {
                    return false;
                }
            }
            return false;
        }

        bool operator<=(const Point<Dim, T>& other) const {
            for (size_t i = 0; i < Dim; i++) {
                if (at(i) > other.at(i)) {
                    return false;
                }
            }
            return true;
        }

        /**
         * Prints the point in json like array format.
         */
        friend std::ostream& operator<<(std::ostream& out, const Point<Dim, T> &point) {
            out << "[";
            for(size_t i = 0; i < point.dim(); i++) {
                if (i+1 != point.dim()) {
                    out << point[i] << ", ";
                } else {
                    out << point[i];
                }
            }
            out << "]";
            return out;
        }

    };

    template<typename T>
    using Point2 = Point<2, T>;

    template<typename T>
    using Point3 = Point<3, T>;

    /**
     * Utility function for easily making a point with 2 dimension
     */
    template<typename T>
    inline Point2<T> make_point2(const T& a, const T& b) {
        return {{a, b}};
    }

    /**
     * Utility function for easily making a point with 3 dimension
     */
    template<typename T>
    inline Point2<T> make_point3(const T& a, const T& b, const T& c) {
        return {{a, b, c}};
    }


    /**
     * Returns the element with the largest value at certain dimension given by
     * the template argument Comp
     */
    template <unsigned int Comp, unsigned int Dim, typename T>
    Point<Dim,T>& max_dim(const Point<Dim,T>& a, const Point<Dim,T>& b) {
        if (a[Comp] < b[Comp]) {
            return b;
        } else {
            return a;
        }
    }

    /**
     * Returns the element with the smallest value at certain dimension given by
     * the template argument Comp
     */
    template <unsigned int Comp, unsigned int Dim, typename T>
    Point<Dim,T>& min_dim(const Point<Dim,T>& a, const Point<Dim,T>& b) {
        if (a[Comp] < b[Comp]) {
            return a;
        } else {
            return b;
        }
    }

    /**
     * Returns the element with the largest value at the dimension given by a
     * runtime index
     */
    template <unsigned int Dim, typename T>
    Point<Dim,T>& max_dim(size_t comp, const Point<Dim,T>& a, const Point<Dim,T>& b) {
        if (a[comp] < b[comp]) {
            return b;
        } else {
            return a;
        }
    }

    /**
     * Returns the element with the smallest value at the dimension given by a
     * runtime index
     */
    template <unsigned int Dim, typename T>
    Point<Dim,T>& min_dim(size_t comp, const Point<Dim,T>& a, const Point<Dim,T>& b) {
        if (a[comp] < b[comp]) {
            return a;
        } else {
            return b;
        }
    }
} // namespace popup
