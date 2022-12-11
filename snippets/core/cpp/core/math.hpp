/**
 * math.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_MATH_H_
#define CORE_MATH_H_

#include "core/core.hpp"

namespace core {
namespace math {

/** ---- Floating point comparison functions ----------------------------------
 * @brief Primitive type floating-point comparison functions.
 * Based on "The Art of Computer Programming", Donald Knuth.
 *
 * isequal
 * @brief Is u essentially equal to v?
 * (u ~ v), iif |u-v| <= eps*min(|u|,|v|)
 */
template<typename T>
core_inline_
bool isequal(const T u, const T v)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    constexpr T epsilon = std::sqrt(std::numeric_limits<T>::epsilon());
    const T abs_u = std::fabs(u);
    const T abs_v = std::fabs(v);

    if (std::fabs(u - v) < epsilon) {
        return true;
    }

    return std::fabs(u - v) <= (epsilon * (abs_u < abs_v ? abs_u : abs_v));
}

/**
 * isless
 * @brief Is u less than v?
 * (u < v), iif (v-u) > eps*max(|u|,|v|)
 */
template<typename T>
core_inline_
bool isless(const T u, const T v)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    constexpr T epsilon = std::sqrt(std::numeric_limits<T>::epsilon());
    const T abs_u = std::fabs(u);
    const T abs_v = std::fabs(v);

    return (v - u) > (epsilon * (abs_u > abs_v ? abs_u : abs_v));
}

/**
 * isgreater
 * @brief Is u greater than v?
 * (u > v), iif (u-v) > eps*max(|u|,|v|)
 */
template<typename T>
core_inline_
bool isgreater(const T u, const T v)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    constexpr T epsilon = std::sqrt(std::numeric_limits<T>::epsilon());
    const T abs_u = std::fabs(u);
    const T abs_v = std::fabs(v);

    return (u - v) > (epsilon * (abs_u > abs_v ? abs_u : abs_v));
}

/** ---- Arithmetic math functions --------------------------------------------
 * fswap<T>
 * @brief Swap the values u and v.
 */
template<typename T>
core_inline_
void fswap(T &u, T &v)
{
    static_assert(std::is_arithmetic<T>::value, "non arithmetic type");
    T tmp = std::move(u);
    u = std::move(v);
    v = std::move(tmp);
}

/**
 * fsign<T>
 * @brief Return the sign of u.
 */
template<typename T>
core_inline_
T fsign(const T u)
{
    static_assert(std::is_arithmetic<T>::value, "non arithmetic type");
    static_assert(std::is_signed<T>::value, "unsigned type");
    constexpr T zero = (T) 0;
    constexpr T plus = (T) 1;
    constexpr T minus = (T) -1;
    return (u < zero ? minus : (u > zero ? plus : zero));
}

template<>
core_inline_
float fsign(const float u)
{
    constexpr float zero = (float) 0;
    constexpr float plus = (float) 1;
    constexpr float minus = (float) -1;
    return (isless(u, zero) ? minus : (isgreater(u, zero) ? plus : zero));
}

template<>
core_inline_
double fsign(const double u)
{
    constexpr double zero = (double) 0;
    constexpr double plus = (double) 1;
    constexpr double minus = (double) -1;
    return (isless(u, zero) ? minus : (isgreater(u, zero) ? plus : zero));
}

/**
 * fabs<T>
 * @brief Return the absolute of u.
 */
template<typename T>
core_inline_
T fabs(const T u)
{
    static_assert(std::is_arithmetic<T>::value, "non arithmetic type");
    static_assert(std::is_signed<T>::value, "unsigned type");
    constexpr T zero = (T) 0;
    return (u < zero ? -u : u);
}

template<>
core_inline_
float fabs(const float u)
{
    constexpr float zero = (float) 0;
    return (isless(u, zero) ? -u : u);
}

template<>
core_inline_
double fabs(const double u)
{
    constexpr double zero = (double) 0;
    return (isless(u, zero) ? -u : u);
}

/**
 * fmin<T>
 * @brief Return the min between u and v.
 */
template<typename T>
core_inline_
T fmin(const T u, const T v)
{
    static_assert(std::is_arithmetic<T>::value, "non arithmetic type");
    return (u < v ? u : v);
}

template<>
core_inline_
float fmin(const float u, const float v)
{
    return (isless(u,v) ? u : v);
}

template<>
core_inline_
double fmin(const double u, const double v)
{
    static_assert(std::is_arithmetic<double>::value, "non arithmetic type");
    return (isless(u,v) ? u : v);
}

/**
 * fmax<T>
 * @brief Return the maximum between u and v.
 */
template<typename T>
core_inline_
T fmax(const T u, const T v)
{
    static_assert(std::is_arithmetic<T>::value, "non arithmetic type");
    return (u < v ? v : u);
}

template<>
core_inline_
float fmax(const float u, const float v)
{
    return (isless(u,v) ? v : u);
}

template<>
core_inline_
double fmax(const double u, const double v)
{
    static_assert(std::is_arithmetic<double>::value, "non arithmetic type");
    return (isless(u,v) ? v : u);
}

/**
 * fclamp<T>
 * @brief Clamp u between lo and hi.
 */
template<typename T>
core_inline_
T fclamp(const T lo, const T hi, const T u)
{
    static_assert(std::is_arithmetic<T>::value, "non arithmetic type");
    return (min(max(lo, u), hi));
}

/**
 * round<T>
 * @brief Round u operator.
 */
template<typename T>
core_inline_
T fround(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    constexpr T zero = 0.0;
    constexpr T half = 0.5;
    return (isless(u, zero) ? std::ceil(u - half) : std::floor(u + half));
}

/**
 * mod<T>
 * @brief Modulo operation of u and v.
 */
template<typename T>
core_inline_
T fmod(const T u, const T v)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    constexpr T zero = 0.0;
    return (isequal(v, zero) ? u : u - v*std::floor(u / v));
}

/**
 * sinc<T>
 * @brief Unnormalized sinc function.
 */
template<typename T>
core_inline_
T fsinc(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    constexpr T zero = 0.0;
    constexpr T one = 1.0;
    return (isequal(u, zero) ? one : std::sin(u) / u);
}

/**
 * cosc<T>
 * @brief Unnormalized cosc function.
 */
template<typename T>
core_inline_
T fcosc(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    constexpr T zero = 0.0;
    return (isequal(u, zero) ? zero : std::cos(u) / u);
}

/**
 * dirac<T>
 * @brief Dirac nascent function is defined as discrete pulse of width e.
 * The true Dirac delta function is the asymptotic value when e->0.
 */
template<typename T>
core_inline_
T dirac(const T u, const T e)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    constexpr T zero = 0.0;
    if (isgreater(e, zero) && isless(2.0*fabs<T>(u), e)) {
        return 2.0 / e;
    }
    return 0.0;
}

/**
 * heaviside<T>
 * @brief Heaviside step function.
 */
template<typename T>
core_inline_
T heaviside(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    constexpr T zero = 0.0;
    constexpr T one = 1.0;
    return (isless(u, zero) ? zero : one);
}

/** ---- Integral math functions ----------------------------------------------
 * kronecker
 * @brief Kronecker delta function.
 */
template<typename T>
core_inline_
T kronecker(const T x, const T y)
{
    static_assert(std::is_integral<T>::value, "non integer type");
    constexpr T zero = (T) 0;
    constexpr T one = (T) 1;
    return (x == y ? one : zero);
}

/**
 * factorial
 * @brief Generalized factorial with an arbitrary step value.
 * Factorial and double factorial are special cases with step=1
 * and step=2 respectivelly.
 */
template<typename T>
core_inline_
T factorial(const T num, const T step = 1)
{
    static_assert(std::is_integral<T>::value, "non integer type");
    T n = num;
    T p = 1;
    while (n > 0) {
        p *= n;
        n -= step;
    }
    return p;
}

/**
 * factorial2
 * @brief Return the double factorial of a number, n!!.
 */
template<typename T>
core_inline_
T factorial2 (const T num)
{
    static_assert(std::is_integral<T>::value, "non integer type");
    return factorial(num, 2);
}

} /* namespace math */
} /* namespace core */

/*
 * C linkage declarations
 */
#undef  __BEGIN_DECLS
#undef  __END_DECLS
#ifdef  __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS   /* empty */
#define __END_DECLS     /* empty */
#endif
__BEGIN_DECLS
__END_DECLS

#endif /* CORE_MATH_H_ */
