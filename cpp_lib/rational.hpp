// Author Marcus Östling, Tomas Möre
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
    /**
     *  Construct a copy of a rational number.
     */
    Rational(const Rational& o) {
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
    inline V numenator() const {
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
      Rational<V> t(this);
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
      Rational<V> t(this);
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
      Rational<V> t(this);
      t *= other;
      return t;
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
      Rational<V> t(this);
      t /= other;
      return t;
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

  };

  /**
   *  Returns the absolute value of given rational number.
   */
  template <class V>
  inline Rational<V> abs(const Rational<V>& a) {
    return Rational<V>(std::abs(a.numenator()),a.denominator());
  }
}
