/*
 * vec2-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_VEC2_SIMD_H_
#define CORE_GEOMETRY_VEC2_SIMD_H_

#include "simd.hpp"

namespace core {
namespace math {

/** ---- simd load/store functions --------------------------------------------
 * simd_load
 * @brief Load 128-bits (2 packed double-precision 64-bit)
 * from the 2d-array (16-byte aligned).
 */
core_inline_
__m128d simd_load(const vec2<double> &v)
{
    return _mm_load_pd(v.data());
}

/**
 * simd_store
 * @brief Store 128-bits (2 packed double-precision 64-bit)
 * into the 2d-array (16-byte aligned).
 */
core_inline_
void simd_store(vec2<double> &v, const __m128d a)
{
    _mm_store_pd(v.data(), a);
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
template<>
core_inline_
vec2<double> &vec2<double>::operator+=(const vec2<double> &other)
{
    const __m128d a_ = simd_load(*this);
    const __m128d b_ = simd_load(other);
    simd_store(*this, _mm_add_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec2<double> &vec2<double>::operator-=(const vec2<double> &other)
{
    const __m128d a_ = simd_load(*this);
    const __m128d b_ = simd_load(other);
    simd_store(*this, _mm_sub_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec2<double> &vec2<double>::operator*=(const vec2<double> &other)
{
    const __m128d a_ = simd_load(*this);
    const __m128d b_ = simd_load(other);
    simd_store(*this, _mm_mul_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec2<double> &vec2<double>::operator/=(const vec2<double> &other)
{
    const __m128d a_ = simd_load(*this);
    const __m128d b_ = simd_load(other);
    simd_store(*this, _mm_div_pd(a_, b_));
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<>
core_inline_
vec2<double> &vec2<double>::operator+=(const double scalar)
{
    const __m128d a_ = simd_load(*this);
    const __m128d b_ = _mm_set1_pd(scalar);
    simd_store(*this, _mm_add_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec2<double> &vec2<double>::operator-=(const double scalar)
{
    const __m128d a_ = simd_load(*this);
    const __m128d b_ = _mm_set1_pd(scalar);
    simd_store(*this, _mm_sub_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec2<double> &vec2<double>::operator*=(const double scalar)
{
    const __m128d a_ = simd_load(*this);
    const __m128d b_ = _mm_set1_pd(scalar);
    simd_store(*this, _mm_mul_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec2<double> &vec2<double>::operator/=(const double scalar)
{
    const __m128d a_ = simd_load(*this);
    const __m128d b_ = _mm_set1_pd(scalar);
    simd_store(*this, _mm_div_pd(a_, b_));
    return *this;
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

#endif /* CORE_GEOMETRY_VEC2_SIMD_H_ */
