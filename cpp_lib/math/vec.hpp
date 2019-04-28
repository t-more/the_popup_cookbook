#pragma once
#include <cmath>
#include <initializer_list>
#include <cstring>
#include <array>

namespace popup {
    const double PI = 3.1415926535897932384626433;

    template <unsigned int Dim, typename T>
    class Vec {
        T vec_[Dim];
    public:

        Vec() {
            vec_ = {0};
        }

        Vec(const T e) {
            vec_ = {e};
        }

        Vec(const T init[Dim]) {
            vec_ = init;
        }

        Vec(const Vec<Dim, T>& other) {
            std::memcpy(vec_, other.vec_, sizeof(vec_));
        }
p
        Vec(const std::array<T, Dim>& other) {
            std::memcpy(vec_, other.begin(), sizeof(vec_));
        }

        // Vec(const std::initializer_list<T> il) {
        //     int i = 0;
        //     for (; i < size() && i < il.size(); i++) {
        //         vec_[i] = il[i];
        //     }
        //     for (; i < size(); i++) {
        //         vec_[i] = 0;
        //     }
        // }


        // Vec(const std::initializer_list<T>& il) {
        //     int i = 0;
        //     for (; i < size() && i < il.size(); i++) {
        //         vec_[i] = il[i];
        //     }
        //     for (; i < size(); i++) {
        //         vec_[i] = 0;
        //     }
        // }

        size_t size() const {
            return Dim;
        }

        // Iterator defenitions

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

        //// Arithmetic operations

        // Addition
        void operator+=(const Vec<Dim, T>& other) {
            auto it = other.cbegin();
            for (auto& e : this) {
                e = *(it++) + e;
            }
        }

        friend Vec<Dim, T> operator+(const Vec<Dim, T>& lhs, const Vec<Dim, T>& rhs) {
            auto res = Vec(lhs);
            res += rhs;
            return res;
        }

        void operator+=(const T& s) {
            for (auto& e : this) {
                e = s + e;
            }
        }

        friend Vec<Dim, T> operator+(const T& lhs, const Vec<Dim, T>& rhs) {
            auto res = Vec(rhs);
            res += lhs;
            return res;
        }

        friend Vec<Dim, T> operator+(const Vec<Dim, T>& lhs, const T& rhs) {
            auto res = Vec(lhs);
            res += rhs;
            return res;
        }

        // Subtraction
        void operator-=(const Vec<Dim, T>& other) {
            auto it = other.cbegin();
            for (auto& e : this) {
                e = *(it++) - e;
            }
        }

        friend Vec<Dim, T> operator-(const Vec<Dim, T>& lhs, const Vec<Dim, T>& rhs) {
            auto res = Vec(lhs);
            res -= rhs;
            return res;
        }

        void operator-=(const T& s) {
            for (auto& e : this) {
                e = e - s;
            }
        }

        friend Vec<Dim, T> operator-(const T& lhs, const Vec<Dim, T>& rhs) {
            auto res = Vec(lhs);
            res -= rhs;
            return res;
        }

        friend Vec<Dim, T> operator-(const Vec<Dim, T>& lhs, const T& rhs) {
            auto res = Vec(lhs);
            res -= rhs;
            return res;
        }

        // Multiplication
        void operator*=(const Vec<Dim, T>& other) {
            auto it = other.cbegin();
            for (auto& e : this) {
                e = *(it++) * e;
            }
        }

        friend Vec<Dim, T> operator*(const Vec<Dim, T>& lhs, const Vec<Dim, T>& rhs) {
            auto res = Vec(lhs);
            res *= rhs;
            return res;
        }

        void operator*=(const T& s) {
            for (auto& e : this) {
                e = e / s;
            }
        }

        friend Vec<Dim, T> operator*(const T& lhs, const Vec<Dim, T>& rhs) {
            auto res = Vec(lhs);
            res /= rhs;
            return res;
        }

        friend Vec<Dim, T> operator*(const Vec<Dim, T>& lhs, const T& rhs) {
            auto res = Vec(lhs);
            res /= rhs;
            return res;
        }

        // Divison
        void operator/=(const Vec<Dim, T>& other) {
            auto it = other.cbegin();
            for (auto& e : this) {
                e = *(it++) / e;
            }
        }

        friend Vec<Dim, T> operator/(const Vec<Dim, T>& lhs, const Vec<Dim, T>& rhs) {
            auto res = Vec(lhs);
            res /= rhs;
            return res;
        }

        void operator/=(const T& s) {
            for (auto& e : this) {
                e = e / s;
            }
        }

        friend Vec<Dim, T> operator/(const T& lhs, const Vec<Dim, T>& rhs) {
            auto res = Vec(lhs);
            res /= rhs;
            return res;
        }

        friend Vec<Dim, T> operator/(const Vec<Dim, T>& lhs, const T& rhs) {
            auto res = Vec(lhs);
            res /= rhs;
            return res;
        }




        // Arithmetic end
        T operator[](size_t i) const {
            return vec_[i];
        }

        T& operator[](size_t i) {
            return vec_[i];
        }


        T dot(const Vec<Dim, T>& other) const {
            T res = T();
            auto it1 = cbegin();
            auto it2 = other->cbegin();
            while (it1 != cend()) {
                res += (*it1) * (*it2);
            }
            return res;
        }

        double norm() const {
            T res = T();
            for (auto& e : this) {
                res += e * e;
            }
            return std::sqrt(res);
        }

        double distance(const Vec<Dim, T>& other) const {
            T res = T();
            auto it1 = cbegin();
            auto it2 = other.cbegin();

            while (it1 != cend()) {
                auto temp = (*it1) - (*it2);
                res += temp * temp;
            }
            return std::sqrt(res);
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

} // namespace popup
