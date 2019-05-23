// Author Marcus Östling, Tomas Möre 2019
#pragma once
#include <numeric>
#include <cmath>
#include <iostream>


namespace popup {

    /**
     * Class that handles basic arithmetic with rational numbers.
     */
    template <class V>
    class Rational {
        V numerator_;
        V denominator_;

        /**
         *  Reduce the number to the smallest rational number.
         */
        inline void reduce() {
            auto d = std::gcd(numerator_, denominator_);
            numerator_ = numerator_ / d;
            denominator_ = denominator_ / d;
        }

    public:

        Rational() : Rational(0,1) {}


        /**
         *  Construct a copy of a rational number.
         */
        Rational(const Rational<V>& o) {
            numerator_ = o.numerator_;
            denominator_ = o.denominator_;
        }

        /**
         *  Construct a rational number given
         *  its numerator and denominator.
         */
        Rational(V n, V d) {
            numerator_ = n * ((0 < d) - (d < 0));
            denominator_ = std::abs(d);
            reduce();
        }

        /**
         *  Construct a rational number given
         *  only the numerator.
         */
        Rational(V n) : Rational(n,1) {}



        /**
         *  Returns numerator.
         */
        inline V numerator() const {
            return numerator_;
        }

        /**
         *  Returns denominator.
         */
        inline V denominator() const {
            return denominator_;
        }

        /**
         *  Addition with another rational number.
         *  Returns a new rational number.
         */
        inline Rational<V> operator+(const Rational<V>& other) const {
            Rational<V> t(*this);
            t += other;
            return t;
        }

        /**
         *  Addition with another rational number.
         *  Set this rational number equal to the sum
         *  of this and the other rational number.
         */
        inline void operator+=(const Rational<V>& other) {
            auto new_numerator = numerator_ * other.denominator_
                + other.numerator_ * denominator_;
            auto new_denominator = denominator_*other.denominator_;
            numerator_ = new_numerator;
            denominator_ = new_denominator;
            reduce();
        }

        /**
         *  Subraction with another rational number.
         *  Returns a new rational number.
         */
        inline Rational<V> operator-() const {
            numerator_ = -numerator_;
            return this;
        }

        /**
         * Returns the negation of this rational number.
         */
        inline Rational<V> operator-(const Rational<V>& other) const {
            Rational<V> t(*this);
            t -= other;
            return t;
        }

        /**
         *  Subtraction with another rational number.
         *  Set this rational number equal to the difference
         *  of this and the other rational number.
         */
        inline void operator-=(const Rational<V>& other) {
            auto new_numerator = numerator_ * other.denominator_
                - other.numerator_ * denominator_;
            auto new_denominator = denominator_*other.denominator_;
            numerator_ = new_numerator;
            denominator_ = new_denominator;
            reduce();
        }

        /**
         *  Multiplication with another rational number.
         *  Returns a new rational number.
         */
        inline Rational<V> operator*(const Rational<V>& other) const {
            Rational<V> t(*this);
            t *= other;
            return t;
        }
        friend Rational<V> operator*(const V& a, const Rational<V>& b) {
            return Rational<V>(a * b.numerator_, b.denominator_);
        }

        /**
         *  Multiplication with another rational number.
         *  Set this rational number equal to the product
         *  of this and the other rational number.
         */
        inline void operator*=(const Rational<V>& other) {
            V new_n = numerator_ * other.numerator_;
            V new_d = denominator_ * other.denominator_;
            numerator_ = new_n;
            denominator_ = new_d;
            reduce();
        }

        /**
         *  Division with another rational number.
         *  Returns a new rational number.
         */
        inline Rational<V> operator/(const Rational<V>& other) const {
            Rational<V> t(*this);
            t /= other;
            return t;
        }

        inline Rational<V> operator/(const V& d) const {
            return Rational<V>(numerator_, denominator_ * 2);
        }

        /**
         *  Division with another rational number.
         *  Set this rational number equal to the quotent
         *  of this and the other rational number.
         */
        inline void operator/=(const Rational<V>& other) {
            auto new_n = numerator_ * other.denominator_;
            auto new_d = denominator_ * other.numerator_;
            numerator_ = new_n * ((0 < new_d) - (new_d < 0));
            denominator_ = std::abs(new_d);
            reduce();
        }

        friend bool operator>(const popup::Rational<V>& a, const popup::Rational<V>& b) {
            return !(a<=b);
        }

        friend bool operator<(const popup::Rational<V>& a, const popup::Rational<V>& b) {
            V q1 = V(0);
            V r1 = a.numerator();
            V q2 = V(0);
            V r2 = b.numerator();
            while (!((q1 < q2 || q2 > q1) || (r1 == 0 || r2 == 0))) {
                q1 = r1 / a.denominator();
                r1 = r1 % a.denominator();
                q2 = r2 / b.denominator();
                r2 = r2 % b.denominator();
            }
            return (!(q1 == q2) && q1 < q2) || r1 == 0;
        }

        friend bool operator<=(const popup::Rational<V>& a, const popup::Rational<V>& b) {
            return a == b || a < b;
        }

        friend bool operator==(const popup::Rational<V>& a, const popup::Rational<V>& b) {
            return a.numerator() == b.numerator() && a.denominator() == b.denominator();
        }

        friend bool operator!=(const popup::Rational<V>& a, const popup::Rational<V>& b) {
            return !(a == b);
        }

        friend std::ostream& operator<< (std::ostream& os, const popup::Rational<V>& a) {
            std::cout << a.numerator_ << "/" << a.denominator_;
            return os;
        }

    };

    /**
     *  Returns the absolute value of given rational number.
     */
    template <class V>
    inline Rational<V> abs(const Rational<V>& a) {
        return Rational<V>(std::abs(a.numerator()),a.denominator());
    }

    // (x:%y) <= (x':%y')  =  x * y' <= x' * y
    //  (x:%y) <  (x':%y')  =  x * y' <  x' * y
}


namespace std {

    template<typename T>
    popup::Rational<T> fabs(const popup::Rational<T>& r) {
        return popup::abs(r);
    }

    template<typename T>
    popup::Rational<T> abs(const popup::Rational<T>& r) {
        return popup::abs(r);
    }


    // This is a bit dangerous, overflows easily
    template<typename T>
    popup::Rational<T> sqrt(const popup::Rational<T>& r) {
        const popup::Rational<T> eps =
            popup::Rational<T>(1, 10000000);

        popup::Rational<T> guess = r / 2;
        popup::Rational<T> err = guess * guess - r;

        int i = 3;
        while (i--) {
            std::cerr << err << endl;
            guess = guess - err / (2 * guess);
            err = guess * guess - r;
        }
        return guess;
    }

    std::ostream& operator<<(std::ostream& os, __int128 t) {
        return os << int64_t(t);
    }
}
