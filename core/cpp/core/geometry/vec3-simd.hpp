/*
 * vec3-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_VEC3_SIMD_H_
#define CORE_GEOMETRY_VEC3_SIMD_H_

#include "simd.hpp"

namespace core {
namespace math {

/** ---- simd load/store functions --------------------------------------------
 * simd_load
 * @brief Load 192-bits (3 packed double precision 64-bit)
 * from a 3d-array (32-byte aligned).
 *
 * @fn _mm256_set_epi64x(__int64 e3, __int64 e2, __int64 e1, __int64 e0)
 * dst[63:0]    := e0
 * dst[127:64]  := e1
 * dst[191:128] := e2
 * dst[255:192] := e3
 *
 * @fn _mm256_maskload_pd(double const * v, __m256i mask)
 * dst[63:0]    := (mask[63]  == 1) ? v[63:0]    : 0
 * dst[127:64]  := (mask[127] == 1) ? v[127:64]  : 0
 * dst[191:128] := (mask[191] == 1) ? v[191:128] : 0
 * dst[255:192] := (mask[255] == 1) ? v[255:192] : 0
 *
 * If any of the bits of the mask is set to zero, the corresponding
 * value from the memory location is not loaded and the corresponding
 * field of the destination vector is set to zero.
 */
core_inline_
__m256d simd_load(const vec3<double> &v)
{
    const __m256i mask_ = _mm256_set_epi64x(0x0,
                                            0xffffffffffffffff,
                                            0xffffffffffffffff,
                                            0xffffffffffffffff);
    return _mm256_maskload_pd(v.data(), mask_);
}

/**
 * simd_store
 * @brief Store 192-bits (3 packed double precision 64-bit)
 * into the 3d-array (32-byte aligned).
 *
 * _mm256_set_epi64x(__int64 e3, __int64 e2, __int64 e1, __int64 e0)
 * dst[63:0]    := e0
 * dst[127:64]  := e1
 * dst[191:128] := e2
 * dst[255:192] := e3
 *
 * _mm256_maskstore_pd(double * v, __m256i mask, __m256d a)
 * v[63:0]    := (mask[63]  == 1) ? a[63:0]
 * v[127:64]  := (mask[127] == 1) ? a[127:64]
 * v[191:128] := (mask[191] == 1) ? a[191:128]
 * v[255:192] := (mask[255] == 1) ? a[255:192]
 *
 * If any of the bits of the mask are set to zero, the corresponding
 * value from the a vector is not loaded, and the corresponding field
 * of the destination vector v is left unchanged.
 */
core_inline_
void simd_store(vec3<double> &v, const __m256d a)
{
    const __m256i mask_ = _mm256_set_epi64x(0x0,
                                            0xffffffffffffffff,
                                            0xffffffffffffffff,
                                            0xffffffffffffffff);
    _mm256_maskstore_pd(v.data(), mask_, a);
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
template<>
core_inline_
vec3<double> &vec3<double>::operator+=(const vec3<double> &other)
{
    const __m256d a_ = simd_load(*this);
    const __m256d b_ = simd_load(other);
    simd_store(*this, _mm256_add_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec3<double> &vec3<double>::operator-=(const vec3<double> &other)
{
    const __m256d a_ = simd_load(*this);
    const __m256d b_ = simd_load(other);
    simd_store(*this, _mm256_sub_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec3<double> &vec3<double>::operator*=(const vec3<double> &other)
{
    const __m256d a_ = simd_load(*this);
    const __m256d b_ = simd_load(other);
    simd_store(*this, _mm256_mul_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec3<double> &vec3<double>::operator/=(const vec3<double> &other)
{
    const __m256d a_ = simd_load(*this);
    const __m256d b_ = simd_load(other);
    simd_store(*this, _mm256_div_pd(a_, b_));
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<>
core_inline_
vec3<double> &vec3<double>::operator+=(const double scalar)
{
    const __m256d a_ = simd_load(*this);
    const __m256d b_ = _mm256_set1_pd(scalar);
    simd_store(*this, _mm256_add_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec3<double> &vec3<double>::operator-=(const double scalar)
{
    const __m256d a_ = simd_load(*this);
    const __m256d b_ = _mm256_set1_pd(scalar);
    simd_store(*this, _mm256_sub_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec3<double> &vec3<double>::operator*=(const double scalar)
{
    const __m256d a_ = simd_load(*this);
    const __m256d b_ = _mm256_set1_pd(scalar);
    simd_store(*this, _mm256_mul_pd(a_, b_));
    return *this;
}

template<>
core_inline_
vec3<double> &vec3<double>::operator/=(const double scalar)
{
    const __m256d a_ = simd_load(*this);
    const __m256d b_ = _mm256_set1_pd(scalar);
    simd_store(*this, _mm256_div_pd(a_, b_));
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

#endif /* CORE_GEOMETRY_VEC3_SIMD_H_ */
