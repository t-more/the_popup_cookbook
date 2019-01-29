#pragma once
#include <numeric>
#include <cmath>

namespace popup {
  template <class V>
  class Rational {
    V numerator_;
    V denominator_;

    inline void reduce() {
      auto d = std::gcd(numerator_, denominator_);
      numerator_ = numerator_ / d;
      denominator_ = denominator_ / d;
    }

  public:
    Rational(const Rational& o) {
      numerator_ = o.numerator_;
      denominator_ = o.denominator_;
    }

    Rational(V n, V d) {
      numerator_ = n;
      denominator_ = d;
    }

    Rational(V n) : Rational(n,1) {}

    inline V numenator() const {
      return numerator_;
    }

    inline V denominator() const {
      return denominator_;
    }

    inline Rational<V> operator+(const Rational<V>& other) const {
      Rational<V> t(this);
      t += other;
      return t;
    }

    inline void operator+=(const Rational<V>& other) {
      auto new_numerator = numerator_ * other.denominator_
                         + other.numerator_ * denominator_;
      auto new_denominator = denominator_*other.denominator_;
      numerator_ = new_numerator;
      denominator_ = new_denominator;
      reduce();
    }

    inline Rational<V> operator-() const {
      numerator_ = -numerator_;
      return this;
    }

    inline Rational<V> operator-(const Rational<V>& other) const {
      Rational<V> t(this);
      t -= other;
      return t;
    }

    inline void operator-=(const Rational<V>& other) {
      auto new_numerator = numerator_ * other.denominator_
                         - other.numerator_ * denominator_;
      auto new_denominator = denominator_*other.denominator_;
      numerator_ = new_numerator;
      denominator_ = new_denominator;
      reduce();
    }

    inline Rational<V> operator*(const Rational<V>& other) const {
      Rational<V> t(this);
      t *= other;
      return t;
    }

    inline Rational<V> operator*=(const Rational<V>& other) {
      auto new_n = numerator_ * other.numerator_;
      auto new_d = denominator_ * other.denominator_;
      numerator_ = new_n;
      denominator_ = new_d;
      reduce();
    }

    inline Rational<V> operator/(const Rational<V>& other) const {
      Rational<V> t(this);
      t /= other;
      return t;
    }

    inline void operator/=(const Rational<V>& other) {
      auto new_n = numerator_ * other.denominator_;
      auto new_d = denominator_ * other.numerator_;
      numerator_ = new_n;
      denominator_ = new_d;
    }

    inline Rational<V> abs(const Rational<V>& a) const {
      return Rational<V>(std::abs(numerator_),denominator_);
    }
  };
}
