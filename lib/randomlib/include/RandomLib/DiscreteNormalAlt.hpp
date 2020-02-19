/**
 * \file DiscreteNormalAlt.hpp
 * \brief Header for DiscreteNormalAlt
 *
 * Sample exactly from the discrete normal distribution (alternate version).
 *
 * Copyright (c) Charles Karney (2013) <charles@karney.com> and licensed
 * under the MIT/X11 License.  For more information, see
 * http://randomlib.sourceforge.net/
 **********************************************************************/

#if !defined(RANDOMLIB_DISCRETENORMALALT_HPP)
#define RANDOMLIB_DISCRETENORMALALT_HPP 1

#include <randomlib/RandomNumber.hpp>
#include <randomlib/UniformInteger.hpp>
#include <limits>

namespace RandomLib {
  /**
   * \brief The discrete normal distribution (alternate version).
   *
   * Sample integers \e i with probability proportional to
   * \f[
   * \exp\biggl[-\frac12\biggl(\frac{i-\mu}{\sigma}\biggr)^2\biggr],
   * \f]
   * where &sigma; and &mu; are given as rationals (the ratio of two integers).
   * The sampling is exact (provided that the random generator is ideal).  The
   * results of this class are UniformInteger objects which represent a
   * contiguous range which is a power of 2<sup>\e bits</sup>.  This can be
   * narrowed down to a specific integer as follows
   * \code
   #include <iostream>
   #include <randomlib/Random.hpp>
   #include <randomlib/UniformInteger.hpp>
   #include <randomlib/DiscreteNormalAlt.hpp>

   int main() {
     RandomLib::Random r;          // Create r
     r.Reseed();                   // and give it a unique seed
     int sigma_num = 7, sigma_den = 1, mu_num = 1, mu_den = 3;
     RandomLib::DiscreteNormalAlt<int,1> d(sigma_num, sigma_den,
                                           mu_num, mu_den);
     for (int i = 0; i < 100; ++i) {
       RandomLib::UniformInteger<int,1> u = d(r);
       std::cout << u << " = ";
       std::cout << u(r) << "\n";
     }
   }
   \endcode
   * prints out 100 samples with &sigma; = 7 and &mu; = 1/3; the samples are
   * first given as a range and then <code>u(r)</code> is used to obtain a
   * specific integer.  In some applications, it might be possible to avoid
   * sampling all the additional digits to get to a specific integer.  (An
   * example would be drawing a sample which satisfies an inequality.)  This
   * version is slower (by a factor of about 4 for \e bits = 1) than
   * DiscreteNormal on a conventional computer, but may be faster when random
   * bits are generated by a slow hardware device.
   *
   * The basic algorithm is the same as for DiscreteNormal.  However randomness
   * in metered out \e bits random bits at a time.  The algorithm uses the
   * least random bits (and is slowest!) when \e bits = 1.  This rationing of
   * random bits also applies to sampling \e j in the expression
   * \f[
   *   x = x_0 + j/\sigma.
   * \f]
   * Rather than sampling a definite value for \e j, which then might be
   * rejected, \e j is sampled using UniformInteger.  UniformInteger uses
   * Lumbroso's method from sampling an integer uniformly in [0, \e m) using at
   * most 2 + log<sub>2</sub>\e m bits on average (for \e bits = 1).  However
   * when a UniformInteger object is first constructed it samples at most 3
   * bits (on average) to obtain a range for \e j which is power of 2.  This
   * allows the algorithm to achieve ideal scaling in the limit of large
   * &sigma; consuming constant + log<sub>2</sub>&sigma; bits on average.  In
   * addition it can deliver the resuls in the form of a UniformInteger
   * consuming a constant number of bits on average (and then about
   * log<sub>2</sub>&sigma; additional bits are required to convert the
   * UniformInteger into an integer sample).  The consumption of random bits
   * (for \e bits = 1) is summarized here \verbatim
                                             min   mean  max
     bits for UniformInteger result          30.4  34.3  35.7
     bits for integer result - log2(sigma)   28.8  31.2  32.5
     toll                                    26.8  29.1  30.4 \endverbatim
   * These results are averaged over many samples.  The "min" results apply
   * when &sigma; is a power of 2; the "max" results apply when &sigma; is
   * slightly more than a power of two; the "mean" results are averaged over
   * &sigma;.  The toll is the excess number of bits over the entropy of the
   * distribution, which is log<sub>2</sub>(2&pi;\e e)/2 +
   * log<sub>2</sub>&sigma; (for &sigma; large).
   *
   * The data for the first two lines in the table above are taken for the blue
   * and red lines in the figure below where the abscissa is the fractional
   * part of log<sub>2</sub>&sigma;
   * \image html
   * discrete-bits.png "Random bits to sample from discrete normal distribution"
   *
   * This class uses a mutable RandomNumber objects.  So a single
   * DiscreteNormalAlt object cannot safely be used by multiple threads.  In a
   * multi-processing environment, each thread should use a thread-specific
   * DiscreteNormalAlt object.
   *
   * @tparam IntType the integer type to use (default int).
   **********************************************************************/
  template<typename IntType = int, int bits = 1>  class DiscreteNormalAlt {
  public:
    /**
     * Constructor.
     *
     * @param[in] sigma_num the numerator of &sigma;.
     * @param[in] sigma_den the denominator of &sigma; (default 1).
     * @param[in] mu_num the numerator of &mu; (default 0).
     * @param[in] mu_den the denominator of &mu; (default 1).
     *
     * This may throw an exception is the parameters are such that overflow is
     * possible.
     **********************************************************************/
    DiscreteNormalAlt(IntType sigma_num, IntType sigma_den = 1,
                   IntType mu_num = 0, IntType mu_den = 1);
    /**
     * Return a sample.
     *
     * @tparam Random the type of the random generator.
     * @param[in,out] r a random generator.
     * @return discrete normal sample in the form of a UniformInteger object.
     **********************************************************************/
    template<class Random>
    UniformInteger<IntType, bits> operator()(Random& r) const;
  private:
    /**
     * sigma = _sig / _d, mu = _imu + _mu / _d, _isig = ceil(sigma)
     **********************************************************************/
    IntType _sig, _mu, _d, _isig, _imu;

    mutable RandomNumber<bits> _p;
    mutable RandomNumber<bits> _q;

    // ceil(n/d) for d > 0
    static IntType iceil(IntType n, IntType d);
    // abs(n) needed because Visual Studio's std::abs has problems
    static IntType iabs(IntType n);
    static IntType gcd(IntType u, IntType v);

    /**
     * Implement outcomes for choosing with prob (\e x + 2\e k) / (2\e k + 2);
     * return:
     * - 1 (succeed unconditionally) with prob (\e m &minus; 2) / \e m,
     * - 0 (succeed with probability x) with prob 1 / \e m,
     * - &minus;1 (fail unconditionally) with prob 1 / \e m.
     **********************************************************************/
    template<class Random> static int Choose(Random& r, int m);

    /**
     * Return true with probability exp(&minus;1/2).
     **********************************************************************/
    template<class Random> bool ExpProbH(Random& r) const;

    /**
     * Return true with probability exp(&minus;<i>n</i>/2).
     **********************************************************************/
    template<class Random> bool ExpProb(Random& r, int n) const;

    /**
     * Return \e n with probability exp(&minus;<i>n</i>/2)
     * (1&minus;exp(&minus;1/2)).
     **********************************************************************/
    template<class Random> int ExpProbN(Random& r) const;

    /**
     * Algorithm B: true with prob exp(-x * (2*k + x) / (2*k + 2)) where
     * x = (xn0 + _d * j) / _sig
     **********************************************************************/
    template<class Random>
    bool B(Random& r, int k, IntType xn0, UniformInteger<IntType, bits>& j)
      const;
  };

  template<typename IntType, int bits>
  DiscreteNormalAlt<IntType, bits>::DiscreteNormalAlt
  (IntType sigma_num, IntType sigma_den, IntType mu_num, IntType mu_den) {
    STATIC_ASSERT(std::numeric_limits<IntType>::is_integer,
                  "DiscreteNormalAlt: invalid integer type IntType");
    STATIC_ASSERT(std::numeric_limits<IntType>::is_signed,
                  "DiscreteNormalAlt: IntType must be a signed type");
    if (!( sigma_num > 0 && sigma_den > 0 && mu_den > 0 ))
      throw RandomErr("DiscreteNormalAlt: need sigma > 0");
    _imu = mu_num / mu_den;
    if (_imu == (std::numeric_limits<IntType>::min)())
      throw RandomErr("DiscreteNormalAlt: abs(mu) too large");
    mu_num -= _imu * mu_den;
    IntType l;
    l = gcd(sigma_num, sigma_den); sigma_num /= l; sigma_den /= l;
    l = gcd(mu_num, mu_den); mu_num /= l; mu_den /= l;
    _isig = iceil(sigma_num, sigma_den);
    l = gcd(sigma_den, mu_den);
    _sig = sigma_num * (mu_den / l);
    _mu = mu_num * (sigma_den / l);
    _d  = sigma_den * (mu_den / l);
    // The rest of the constructor tests for possible overflow
    // Check for overflow in computing member variables
    IntType maxint = (std::numeric_limits<IntType>::max)();
    if (!( mu_den / l <= maxint / sigma_num &&
           iabs(mu_num) <= maxint / (sigma_den / l) &&
           mu_den / l <= maxint / sigma_den ))
      throw RandomErr("DiscreteNormalAlt: sigma or mu overflow");
    if (!UniformInteger<IntType, bits>::Check(_isig, _d))
      throw RandomErr("DiscreteNormalAlt: overflow in UniformInteger");
    // The probability that k =  kmax is about 10^-543.
    int kmax = 50;
    // Check that max plausible result fits in an IntType, i.e.,
    // _isig * (kmax + 1) + abs(_imu) does not lead to overflow.
    if (!( kmax + 1 <=  maxint / _isig &&
           _isig * (kmax + 1) <= maxint - iabs(_imu) ))
      throw RandomErr("DiscreteNormalAlt: possible overflow a");
    // Check xn0 = _sig * k + s * _mu;
    if (!( kmax <= maxint / _sig &&
           _sig * kmax <= maxint - iabs(_mu) ))
      throw RandomErr("DiscreteNormalAlt: possible overflow b");
    // Check for overflow in RandomNumber::LessThan(..., UniformInteger& j)
    // p0 * b, p0 = arg2 = xn0 = _d - 1
    // c *= b, c = arg3 = _d
    // digit(D, k) * q, digit(D, k) = b - 1, q = arg4 = _sig
    if (!( _d <= maxint >> bits ))
      throw std::runtime_error("DiscreteNormalAlt: overflow in RandomNumber a");
    if (!( (IntType(1) << bits) - 1  <= maxint / _sig ))
      throw std::runtime_error("DiscreteNormalAlt: overflow in RandomNumber b");
  }

  template<typename IntType, int bits> template<class Random>
  UniformInteger<IntType, bits>
  DiscreteNormalAlt<IntType, bits>::operator()(Random& r) const {
    for (;;) {
      int k = ExpProbN(r);
      if (!ExpProb(r, k * (k - 1))) continue;
      UniformInteger<IntType, bits> j(r, _isig);
      IntType
        s = r.Boolean() ? -1 : 1,
        xn0 = _sig * IntType(k) + s * _mu,
        i0 = iceil(xn0, _d);     // i = i0 + j
      xn0 = i0 * _d - xn0;       // xn = xn0 + _d * j
      if (!j.LessThan(r, _sig - xn0, _d) ||
          (k == 0 && s < 0 && !j.GreaterThan(r, -xn0, _d))) continue;
      int h = k + 1; while (h-- && B(r, k, xn0, j));
      if (!(h < 0)) continue;
      j.Add(i0 + s * _imu);
      if (s < 0) j.Negate();
      return j;
    }
  }

  template<typename IntType, int bits>
  IntType DiscreteNormalAlt<IntType, bits>::iceil(IntType n, IntType d)
  { IntType k = n / d; return k + (k * d < n ? 1 : 0); }

  template<typename IntType, int bits>
  IntType DiscreteNormalAlt<IntType, bits>::iabs(IntType n)
  { return n < 0 ? -n : n; }

  template<typename IntType, int bits>
  IntType DiscreteNormalAlt<IntType, bits>::gcd(IntType u, IntType v) {
    // Knuth, TAOCP, vol 2, 4.5.2, Algorithm A
    u = iabs(u); v = iabs(v);
    while (v > 0) { IntType r = u % v; u = v; v = r; }
    return u;
  }

  template<typename IntType, int bits> template<class Random>
  int DiscreteNormalAlt<IntType, bits>::Choose(Random& r, int m) {
    // Limit base to 2^15 to avoid integer overflow
    const int b = bits > 15 ? 15 : bits;
    const unsigned mask = (1u << b) - 1;
    int n1 = m - 2, n2 = m - 1;
    // Evaluate u < n/m where u is a random real number in [0,1).  Write u =
    // (d + u') / 2^b where d is a random integer in [0,2^b) and u' is in
    // [0,1).  Then u < n/m becomes u' < n'/m where n' = 2^b * n - d * m and
    // exit if n' <= 0 (false) or n' >= m (true).
    for (;;) {
      int d = (mask & RandomNumber<bits>::RandomDigit(r)) * m;
      n1 = (std::max)((n1 << b) - d, 0);
      if (n1 >= m) return 1;
      n2 = (std::min)((n2 << b) - d, m);
      if (n2 <= 0) return -1;
      if (n1 == 0 && n2 == m) return 0;
    }
  }

  template<typename IntType, int bits> template<class Random>
  bool DiscreteNormalAlt<IntType, bits>::ExpProbH(Random& r) const {
    _p.Init();
    if (_p.Digit(r, 0) >> (bits - 1)) return true;
    for (;;) {
      _q.Init(); if (!_q.LessThan(r, _p)) return false;
      _p.Init(); if (!_p.LessThan(r, _q)) return true;
    }
  }

  template<typename IntType, int bits> template<class Random>
  bool DiscreteNormalAlt<IntType, bits>::ExpProb(Random& r, int n) const {
    while (n-- > 0) { if (!ExpProbH(r)) return false; }
    return true;
  }

  template<typename IntType, int bits> template<class Random>
  int DiscreteNormalAlt<IntType, bits>::ExpProbN(Random& r) const {
    int n = 0;
    while (ExpProbH(r)) ++n;
    return n;
  }

  template<typename IntType, int bits> template<class Random> bool
  DiscreteNormalAlt<IntType, bits>::B(Random& r, int k, IntType xn0,
                                      UniformInteger<IntType, bits>& j)
    const {
    int n = 0, m = 2 * k + 2, f;
    for (;; ++n) {
      if ( ((f = k ? 0 : Choose(r, m)) < 0) ||
           (_q.Init(),
            !(n ? _q.LessThan(r, _p) : _q.LessThan(r, xn0, _d, _sig, j))) ||
           ((f = k ? Choose(r, m) : f) < 0) ||
           (f == 0 && (_p.Init(), !_p.LessThan(r, xn0, _d, _sig, j))) )
        break;
      _p.swap(_q);              // an efficient way of doing p = q
    }
    return (n % 2) == 0;
  }

} // namespace RandomLib

#endif  // RANDOMLIB_DISCRETENORMALALT_HPP
