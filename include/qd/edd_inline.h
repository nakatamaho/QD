/*
 * include/edd_inline.h
 *
 * Inline building blocks for edd_real based on native _Float64x limbs.
 */
#ifndef _QD_EDD_INLINE_H
#define _QD_EDD_INLINE_H

#include <qd/edd_real.h>

#ifndef QD_INLINE
#define inline
#endif

namespace edd {

inline _Float64x fabsx(_Float64x x) { return __builtin_fabsf64x(x); }
inline _Float64x sqrtx(_Float64x x) { return __builtin_sqrtf64x(x); }
inline _Float64x floorx(_Float64x x) { return __builtin_floorf64x(x); }
inline _Float64x ldexpx(_Float64x x, int e) { return __builtin_ldexpf64x(x, e); }
inline _Float64x copysignx(_Float64x x, _Float64x y) {
  return __builtin_copysignf64x(x, y);
}
inline _Float64x log10x(_Float64x x) { return __builtin_log10f64x(x); }

inline bool isnanx(_Float64x x) { return __builtin_isnan(x); }
inline bool isinfx(_Float64x x) { return __builtin_isinf_sign(x) != 0; }
inline bool isfinitex(_Float64x x) { return __builtin_isfinite(x); }

inline _Float64x d_nan() { return __builtin_nanf64x(""); }
inline _Float64x d_inf() { return __builtin_huge_valf64x(); }

inline _Float64x splitter() {
  return ldexpx((_Float64x) 1.0, QD_EDD_SPLIT_BITS) + (_Float64x) 1.0;
}

inline _Float64x split_thresh() {
  return ldexpx((_Float64x) 1.0, QD_EDD_FLT64X_MAX_EXP - QD_EDD_SPLIT_SCALE_BITS);
}

inline _Float64x div_rescale_thresh() {
  return ldexpx((_Float64x) 1.0,
      QD_EDD_FLT64X_MAX_EXP - QD_EDD_FLT64X_MANT_DIG);
}

inline _Float64x sqrt_rescale_thresh() {
  return ldexpx((_Float64x) 1.0, QD_EDD_FLT64X_MAX_EXP - 3);
}

inline _Float64x quick_two_sum(_Float64x a, _Float64x b, _Float64x &err) {
  _Float64x s = a + b;
  err = b - (s - a);
  return s;
}

inline _Float64x quick_two_diff(_Float64x a, _Float64x b, _Float64x &err) {
  _Float64x s = a - b;
  err = (a - s) - b;
  return s;
}

inline _Float64x two_sum(_Float64x a, _Float64x b, _Float64x &err) {
  _Float64x s = a + b;
  _Float64x bb = s - a;
  err = (a - (s - bb)) + (b - bb);
  return s;
}

inline _Float64x two_diff(_Float64x a, _Float64x b, _Float64x &err) {
  _Float64x s = a - b;
  _Float64x bb = s - a;
  err = (a - (s - bb)) - (b + bb);
  return s;
}

inline void split(_Float64x a, _Float64x &hi, _Float64x &lo) {
  _Float64x temp;
  if (fabsx(a) > split_thresh()) {
    a = ldexpx(a, -QD_EDD_SPLIT_SCALE_BITS);
    temp = splitter() * a;
    hi = temp - (temp - a);
    lo = a - hi;
    hi = ldexpx(hi, QD_EDD_SPLIT_SCALE_BITS);
    lo = ldexpx(lo, QD_EDD_SPLIT_SCALE_BITS);
  } else {
    temp = splitter() * a;
    hi = temp - (temp - a);
    lo = a - hi;
  }
}

inline _Float64x two_prod(_Float64x a, _Float64x b, _Float64x &err) {
  _Float64x a_hi, a_lo, b_hi, b_lo;
  _Float64x p = a * b;
  split(a, a_hi, a_lo);
  split(b, b_hi, b_lo);
  err = ((a_hi * b_hi - p) + a_hi * b_lo + a_lo * b_hi) + a_lo * b_lo;
  return p;
}

inline _Float64x two_sqr(_Float64x a, _Float64x &err) {
  _Float64x hi, lo;
  _Float64x q = a * a;
  split(a, hi, lo);
  err = ((hi * hi - q) + ((_Float64x) 2.0) * hi * lo) + lo * lo;
  return q;
}

inline _Float64x nint(_Float64x x) {
  if (x == floorx(x))
    return x;
  return floorx(x + (_Float64x) 0.5);
}

template <class A, class B>
inline bool comparison_eq(const A &a, const B &b) {
  edd_real ea(a);
  edd_real eb(b);
  if (ea.isnan() || eb.isnan())
    return false;
  if (ea.isinf() || eb.isinf())
    return ea[0] == eb[0];
  edd_real d = ea - eb;
  return d.is_zero();
}

template <class A, class B>
inline bool comparison_lt(const A &a, const B &b) {
  edd_real ea(a);
  edd_real eb(b);
  if (ea.isnan() || eb.isnan())
    return false;
  if (ea.isinf() || eb.isinf())
    return ea[0] < eb[0];
  edd_real d = ea - eb;
  return d.is_negative();
}

template <class A, class B>
inline bool comparison_gt(const A &a, const B &b) {
  edd_real ea(a);
  edd_real eb(b);
  if (ea.isnan() || eb.isnan())
    return false;
  if (ea.isinf() || eb.isinf())
    return ea[0] > eb[0];
  edd_real d = ea - eb;
  return d.is_positive();
}

template <class A, class B>
inline bool comparison_le(const A &a, const B &b) {
  edd_real ea(a);
  edd_real eb(b);
  if (ea.isnan() || eb.isnan())
    return false;
  if (ea.isinf() || eb.isinf())
    return ea[0] <= eb[0];
  edd_real d = ea - eb;
  return d.is_zero() || d.is_negative();
}

template <class A, class B>
inline bool comparison_ge(const A &a, const B &b) {
  edd_real ea(a);
  edd_real eb(b);
  if (ea.isnan() || eb.isnan())
    return false;
  if (ea.isinf() || eb.isinf())
    return ea[0] >= eb[0];
  edd_real d = ea - eb;
  return d.is_zero() || d.is_positive();
}

} // namespace edd

inline bool edd_real::isnan() const {
  return edd::isnanx(x[0]) || edd::isnanx(x[1]);
}

inline bool edd_real::isfinite() const {
  return edd::isfinitex(x[0]);
}

inline bool edd_real::isinf() const {
  return edd::isinfx(x[0]);
}

inline edd_real edd_real::add(_Float64x a, _Float64x b) {
  _Float64x s, e;
  s = edd::two_sum(a, b, e);
  return edd_real(s, e);
}

inline edd_real operator+(const edd_real &a, _Float64x b) {
  _Float64x s1, s2;
  s1 = edd::two_sum(a.x[0], b, s2);
  s2 += a.x[1];
  s1 = edd::quick_two_sum(s1, s2, s2);
  return edd_real(s1, s2);
}

inline edd_real operator+(_Float64x a, const edd_real &b) {
  return b + a;
}

inline edd_real operator+(const edd_real &a, const edd_real &b) {
  _Float64x s1, s2, t1, t2;
  s1 = edd::two_sum(a.x[0], b.x[0], s2);
  t1 = edd::two_sum(a.x[1], b.x[1], t2);
  s2 += t1;
  s1 = edd::quick_two_sum(s1, s2, s2);
  s2 += t2;
  s1 = edd::quick_two_sum(s1, s2, s2);
  return edd_real(s1, s2);
}

inline edd_real &edd_real::operator+=(_Float64x a) {
  _Float64x s1, s2;
  s1 = edd::two_sum(x[0], a, s2);
  s2 += x[1];
  x[0] = edd::quick_two_sum(s1, s2, x[1]);
  return *this;
}

inline edd_real &edd_real::operator+=(const edd_real &a) {
  _Float64x s1, s2, t1, t2;
  s1 = edd::two_sum(x[0], a.x[0], s2);
  t1 = edd::two_sum(x[1], a.x[1], t2);
  s2 += t1;
  s1 = edd::quick_two_sum(s1, s2, s2);
  s2 += t2;
  x[0] = edd::quick_two_sum(s1, s2, x[1]);
  return *this;
}

inline edd_real edd_real::sub(_Float64x a, _Float64x b) {
  _Float64x s, e;
  s = edd::two_diff(a, b, e);
  return edd_real(s, e);
}

inline edd_real operator-(const edd_real &a, _Float64x b) {
  _Float64x s1, s2;
  s1 = edd::two_diff(a.x[0], b, s2);
  s2 += a.x[1];
  s1 = edd::quick_two_sum(s1, s2, s2);
  return edd_real(s1, s2);
}

inline edd_real operator-(_Float64x a, const edd_real &b) {
  _Float64x s1, s2;
  s1 = edd::two_diff(a, b.x[0], s2);
  s2 -= b.x[1];
  s1 = edd::quick_two_sum(s1, s2, s2);
  return edd_real(s1, s2);
}

inline edd_real operator-(const edd_real &a, const edd_real &b) {
  _Float64x s1, s2, t1, t2;
  s1 = edd::two_diff(a.x[0], b.x[0], s2);
  t1 = edd::two_diff(a.x[1], b.x[1], t2);
  s2 += t1;
  s1 = edd::quick_two_sum(s1, s2, s2);
  s2 += t2;
  s1 = edd::quick_two_sum(s1, s2, s2);
  return edd_real(s1, s2);
}

inline edd_real &edd_real::operator-=(_Float64x a) {
  _Float64x s1, s2;
  s1 = edd::two_diff(x[0], a, s2);
  s2 += x[1];
  x[0] = edd::quick_two_sum(s1, s2, x[1]);
  return *this;
}

inline edd_real &edd_real::operator-=(const edd_real &a) {
  _Float64x s1, s2, t1, t2;
  s1 = edd::two_diff(x[0], a.x[0], s2);
  t1 = edd::two_diff(x[1], a.x[1], t2);
  s2 += t1;
  s1 = edd::quick_two_sum(s1, s2, s2);
  s2 += t2;
  x[0] = edd::quick_two_sum(s1, s2, x[1]);
  return *this;
}

inline edd_real edd_real::operator-() const {
  return edd_real(-x[0], -x[1]);
}

inline edd_real edd_real::mul(_Float64x a, _Float64x b) {
  _Float64x p, e;
  p = edd::two_prod(a, b, e);
  return edd_real(p, e);
}

inline edd_real ldexp(const edd_real &a, int exp) {
  return edd_real(edd::ldexpx(a.x[0], exp), edd::ldexpx(a.x[1], exp));
}

inline edd_real mul_pwr2(const edd_real &a, _Float64x b) {
  return edd_real(a.x[0] * b, a.x[1] * b);
}

inline edd_real operator*(const edd_real &a, _Float64x b) {
  _Float64x p1, p2;
  p1 = edd::two_prod(a.x[0], b, p2);
  p2 += a.x[1] * b;
  p1 = edd::quick_two_sum(p1, p2, p2);
  return edd_real(p1, p2);
}

inline edd_real operator*(_Float64x a, const edd_real &b) {
  return b * a;
}

inline edd_real operator*(const edd_real &a, const edd_real &b) {
  _Float64x p1, p2;
  p1 = edd::two_prod(a.x[0], b.x[0], p2);
  p2 += a.x[0] * b.x[1] + a.x[1] * b.x[0];
  p1 = edd::quick_two_sum(p1, p2, p2);
  return edd_real(p1, p2);
}

inline edd_real &edd_real::operator*=(_Float64x a) {
  _Float64x p1, p2;
  p1 = edd::two_prod(x[0], a, p2);
  p2 += x[1] * a;
  x[0] = edd::quick_two_sum(p1, p2, x[1]);
  return *this;
}

inline edd_real &edd_real::operator*=(const edd_real &a) {
  _Float64x p1, p2;
  p1 = edd::two_prod(x[0], a.x[0], p2);
  p2 += x[0] * a.x[1] + x[1] * a.x[0];
  x[0] = edd::quick_two_sum(p1, p2, x[1]);
  return *this;
}

inline bool edd_real_div_needs_rescale(_Float64x a_hi) {
  return edd::fabsx(a_hi) > edd::div_rescale_thresh();
}

inline edd_real edd_real::div(_Float64x a, _Float64x b) {
  _Float64x q1, q2;
  _Float64x p1, p2;
  _Float64x s, e;

  const bool rescale = edd_real_div_needs_rescale(a);
  const _Float64x aa = rescale ? edd::ldexpx(a, -QD_EDD_FLT64X_MANT_DIG) : a;

  q1 = aa / b;
  p1 = edd::two_prod(q1, b, p2);
  s = edd::two_diff(aa, p1, e);
  e -= p2;
  q2 = (s + e) / b;
  s = edd::quick_two_sum(q1, q2, e);

  edd_real r(s, e);
  return rescale ? mul_pwr2(r, edd::ldexpx((_Float64x) 1.0, QD_EDD_FLT64X_MANT_DIG)) : r;
}

inline edd_real operator/(const edd_real &a, _Float64x b) {
  _Float64x q1, q2;
  _Float64x p1, p2;
  _Float64x s, e;
  edd_real r;

  const bool rescale = edd_real_div_needs_rescale(a.x[0]);
  const edd_real aa = rescale ? mul_pwr2(a, edd::ldexpx((_Float64x) 1.0, -QD_EDD_FLT64X_MANT_DIG)) : a;

  q1 = aa.x[0] / b;
  p1 = edd::two_prod(q1, b, p2);
  s = edd::two_diff(aa.x[0], p1, e);
  e += aa.x[1];
  e -= p2;
  q2 = (s + e) / b;
  r.x[0] = edd::quick_two_sum(q1, q2, r.x[1]);

  return rescale ? mul_pwr2(r, edd::ldexpx((_Float64x) 1.0, QD_EDD_FLT64X_MANT_DIG)) : r;
}

inline edd_real operator/(const edd_real &a, const edd_real &b) {
  _Float64x q1, q2, q3;
  edd_real r;

  const bool rescale = edd_real_div_needs_rescale(a.x[0]);
  const edd_real aa = rescale ? mul_pwr2(a, edd::ldexpx((_Float64x) 1.0, -QD_EDD_FLT64X_MANT_DIG)) : a;

  q1 = aa.x[0] / b.x[0];
  r = aa - q1 * b;
  q2 = r.x[0] / b.x[0];
  r -= q2 * b;
  q3 = r.x[0] / b.x[0];

  q1 = edd::quick_two_sum(q1, q2, q2);
  r = edd_real(q1, q2) + q3;
  return rescale ? mul_pwr2(r, edd::ldexpx((_Float64x) 1.0, QD_EDD_FLT64X_MANT_DIG)) : r;
}

inline edd_real operator/(_Float64x a, const edd_real &b) {
  return edd_real(a) / b;
}

inline edd_real &edd_real::operator/=(_Float64x a) {
  *this = *this / a;
  return *this;
}

inline edd_real &edd_real::operator/=(const edd_real &a) {
  *this = *this / a;
  return *this;
}

inline edd_real sqr(const edd_real &a) {
  _Float64x p1, p2, s2;
  p1 = edd::two_sqr(a.x[0], p2);
  p2 += ((_Float64x) 2.0) * a.x[0] * a.x[1];
  p2 += a.x[1] * a.x[1];
  p1 = edd::quick_two_sum(p1, p2, s2);
  return edd_real(p1, s2);
}

inline edd_real edd_real::sqr(_Float64x a) {
  _Float64x p1, p2;
  p1 = edd::two_sqr(a, p2);
  return edd_real(p1, p2);
}

inline edd_real edd_real::operator^(int n) const {
  return npwr(*this, n);
}

inline edd_real &edd_real::operator=(_Float64x a) {
  x[0] = a;
  x[1] = (_Float64x) 0.0;
  return *this;
}

inline edd_real &edd_real::operator=(double a) {
  x[0] = (_Float64x) a;
  x[1] = (_Float64x) 0.0;
  return *this;
}

inline edd_real &edd_real::operator=(int a) {
  x[0] = (_Float64x) a;
  x[1] = (_Float64x) 0.0;
  return *this;
}

inline bool operator==(const edd_real &a, _Float64x b) {
  return edd::comparison_eq(a, b);
}

inline bool operator==(_Float64x a, const edd_real &b) {
  return edd::comparison_eq(a, b);
}

inline bool operator==(const edd_real &a, const edd_real &b) {
  return edd::comparison_eq(a, b);
}

inline bool operator!=(const edd_real &a, _Float64x b) {
  return !(a == b);
}

inline bool operator!=(_Float64x a, const edd_real &b) {
  return !(a == b);
}

inline bool operator!=(const edd_real &a, const edd_real &b) {
  return !(a == b);
}

inline bool operator<(const edd_real &a, _Float64x b) {
  return edd::comparison_lt(a, b);
}

inline bool operator<(_Float64x a, const edd_real &b) {
  return edd::comparison_lt(a, b);
}

inline bool operator<(const edd_real &a, const edd_real &b) {
  return edd::comparison_lt(a, b);
}

inline bool operator>(const edd_real &a, _Float64x b) {
  return edd::comparison_gt(a, b);
}

inline bool operator>(_Float64x a, const edd_real &b) {
  return edd::comparison_gt(a, b);
}

inline bool operator>(const edd_real &a, const edd_real &b) {
  return edd::comparison_gt(a, b);
}

inline bool operator<=(const edd_real &a, _Float64x b) {
  return edd::comparison_le(a, b);
}

inline bool operator<=(_Float64x a, const edd_real &b) {
  return edd::comparison_le(a, b);
}

inline bool operator<=(const edd_real &a, const edd_real &b) {
  return edd::comparison_le(a, b);
}

inline bool operator>=(const edd_real &a, _Float64x b) {
  return edd::comparison_ge(a, b);
}

inline bool operator>=(_Float64x a, const edd_real &b) {
  return edd::comparison_ge(a, b);
}

inline bool operator>=(const edd_real &a, const edd_real &b) {
  return edd::comparison_ge(a, b);
}

inline edd_real abs(const edd_real &a) {
  return (a[0] < (_Float64x) 0.0) ? -a : a;
}

inline edd_real fabs(const edd_real &a) {
  return abs(a);
}

inline bool edd_real::is_zero() const {
  return x[0] == (_Float64x) 0.0 && x[1] == (_Float64x) 0.0;
}

inline bool edd_real::is_one() const {
  return x[0] == (_Float64x) 1.0 && x[1] == (_Float64x) 0.0;
}

inline bool edd_real::is_positive() const {
  return x[0] > (_Float64x) 0.0 ||
      (x[0] == (_Float64x) 0.0 && x[1] > (_Float64x) 0.0);
}

inline bool edd_real::is_negative() const {
  return x[0] < (_Float64x) 0.0 ||
      (x[0] == (_Float64x) 0.0 && x[1] < (_Float64x) 0.0);
}

inline _Float64x to_float64x(const edd_real &a) {
  return a[0] + a[1];
}

inline double to_double(const edd_real &a) {
  return static_cast<double>(a[0] + a[1]);
}

inline int to_int(const edd_real &a) {
  return static_cast<int>(a[0] + a[1]);
}

#endif /* _QD_EDD_INLINE_H */
