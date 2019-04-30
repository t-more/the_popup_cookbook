#pragma once
#include <cmath>
#include <initializer_list>
#include <cstring>
#include <array>

namespace popup {
    const double PI = 3.1415926535897932384626433;

    template <unsigned int Dim, typename T>
    class Point {
        T vec_[Dim];
    public:
        Point() {
            std::fill(vec_, vec_+Dim, 0);
        }
        Point(const T e) {
            vec_ = {e};
        }

        Point(const T init[Dim]) {
            vec_ = init;
        }

        Point(const Point<Dim, T>& other) {
            std::memcpy(vec_, other.vec_, sizeof(vec_));
        }

        Point(const std::array<T, Dim>& other) {
            std::memcpy(vec_, other.begin(), sizeof(vec_));
        }

        size_t dim() const {
            return Dim;
        }
        size_t size() const {
            return Dim;
        }

        T* begin() {
            return vec_;
        }

        T* end() {
            return vec_ + size();
        }

        const T * cbegin() const {
            return vec_;
        }

        const T * cend() const {
            return vec_ + size();
        }

        T at(const size_t i) const {
            return vec_[i];
        }

        T& at(const size_t i) {
            return vec_[i];
        }

        T operator[](const size_t i) const {
            return at(i);
        }

        T& operator[](const size_t i) {
            return at(i);
        }

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
                e = *(it++) - e;
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
            for (auto& e : this) {
                e = *(it++) * e;
            }
        }

        friend Point<Dim, T> operator*(const Point<Dim, T>& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(lhs);
            res *= rhs;
            return res;
        }

        void operator*=(const T& s) {
            for (auto& e : this) {
                e = e / s;
            }
        }

        friend Point<Dim, T> operator*(const T& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(lhs);
            res /= rhs;
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
            for (auto& e : this) {
                e = *(it++) / e;
            }
        }

        friend Point<Dim, T> operator/(const Point<Dim, T>& lhs, const Point<Dim, T>& rhs) {
            auto res = Point(lhs);
            res /= rhs;
            return res;
        }

        void operator/=(const T& s) {
            for (auto& e : this) {
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

        T distance_to(const Point<Dim, T>& other) const {
            T res = T();
            auto it1 = cbegin();
            auto it2 = other.cbegin();

            while (it1 != cend()) {
                auto temp = (*it1) - (*it2);
                res += temp * temp;
            }
            return std::sqrt(res);
        }

        bool comparable(const Point<Dim, T>& other, const T epsilon = 1e-9) {
            for (size_t i = 0; i < Dim; i++) {
                if (!(std::abs((*this)[i] - other[i]) < epsilon)) {
                    return false;
                }
            }
            return true;
        }

    };

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

        /**
         * Gives the deteerminant of two vectors together with the 1 vector
         */
        T det(const Vec<Dim, T>& other) {
            T res = T();
            for (size_t i = 1; i < dim(); i++) {
                res += (*this)[i % dim()] * other[(i + 1) % dim()];
                res -= (*this)[i % dim()] * other[(i - 1) % dim()];
            }
            return res;
        }

        // Iterator defenitions

        inline T* begin() {
            return point_.begin();
        }

        inline T* end() {
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
                res += (*it1) * (*it2);
            }
            return res;
        }

        T norm() const {
            T res = T();
            for (auto& e : this) {
                res += e * e;
            }
            return std::sqrt(res);
        }

        /**
         * Mutates the vector into a normalized form.
         */
        void normalize() const {
            point_ /= norm();
        }

        /**
         * Mutates the vector into a normalized form.
         */
        Vec<Dim, T> normalized() const {
            return (*this) / norm();
        }

        bool comparable(const Vec<Dim, T>& other, const T epsilon = 1e-9) {
            return point_.comparable(other.point_, epsilon);
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
        out << "[";
        for(size_t i = 0; i < Dim; i++) {
            if (i+1 != Dim) {
                out << vec[i] << ", ";
            } else {
                out << vec[i];
            }
        }
        out << "]";

        return out;
    }

    template <unsigned int Comp, unsigned int Dim, typename T>
    Vec<Dim,T>& max_dim(const Vec<Dim,T>& a, const Vec<Dim,T>& b) {
        if (a[Comp] < b[Comp]) {
            return b;
        } else {
            return a;
        }
    }

    template <unsigned int Comp, unsigned int Dim, typename T>
    Vec<Dim,T>& min_dim(const Vec<Dim,T>& a, const Vec<Dim,T>& b) {
        if (a[Comp] < b[Comp]) {
            return a;
        } else {
            return b;
        }
    }
} // namespace popup
