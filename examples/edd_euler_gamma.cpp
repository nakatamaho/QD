/*
 * examples/edd_euler_gamma.cpp
 *
 * Compute H_n - log(n) with edd_real and use Euler-Maclaurin correction
 * terms to estimate the Euler-Mascheroni constant.
 */
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#include <qd/edd_real.h>

namespace {

long parse_n(const char *s, long default_value) {
  if (s == NULL) {
    return default_value;
  }

  char *end = NULL;
  long value = std::strtol(s, &end, 10);
  if (end == s || *end != '\0' || value <= 0) {
    return default_value;
  }

  return value;
}

edd_real harmonic_number(long n) {
  edd_real sum((edd_word) 0.0);
  for (long k = 1; k <= n; ++k) {
    sum += edd_real((edd_word) 1.0) / edd_real((edd_word) k);
  }
  return sum;
}

edd_real gamma_from_harmonic(long n) {
  edd_real nn((edd_word) n);
  edd_real hn = harmonic_number(n);
  return hn - log(nn);
}

edd_real gamma_with_correction(long n) {
  edd_real nn((edd_word) n);
  edd_real inv_n = edd_real((edd_word) 1.0) / nn;
  edd_real inv_n2 = sqr(inv_n);
  edd_real inv_n4 = sqr(inv_n2);
  edd_real inv_n6 = inv_n4 * inv_n2;
  edd_real raw = gamma_from_harmonic(n);

  return raw
      - inv_n / (edd_word) 2.0
      + inv_n2 / (edd_word) 12.0
      - inv_n4 / (edd_word) 120.0
      + inv_n6 / (edd_word) 252.0;
}

} // namespace

int main(int argc, char **argv) {
  long n = 1000000;

  for (int i = 1; i < argc; ++i) {
    n = parse_n(argv[i], n);
  }

  const edd_real gamma_ref(
      "5.77215664901532860606512090082402431042e-01");

  std::cout << std::setprecision(edd_real::_ndigits);

  edd_real raw = gamma_from_harmonic(n);
  edd_real corrected = gamma_with_correction(n);
  edd_real raw_err = abs(raw - gamma_ref);
  edd_real corrected_err = abs(corrected - gamma_ref);

  std::cout << "n                 = " << n << '\n';
  std::cout << "H_n - log(n)      = " << raw << '\n';
  std::cout << "Euler-Maclaurin   = " << corrected << '\n';
  std::cout << "gamma reference   = " << gamma_ref << '\n';
  std::cout << "raw abs error     = " << raw_err << '\n';
  std::cout << "corrected error   = " << corrected_err << '\n';
  std::cout << "raw error / eps   = "
            << static_cast<long double>(to_float64x(raw_err) / edd_real::_eps)
            << '\n';
  std::cout << "corr error / eps  = "
            << static_cast<long double>(to_float64x(corrected_err)
                / edd_real::_eps)
            << '\n';

  return 0;
}
