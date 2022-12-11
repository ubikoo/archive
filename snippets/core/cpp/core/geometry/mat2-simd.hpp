/*
 * mat2-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_MAT2_SIMD_H_
#define CORE_GEOMETRY_MAT2_SIMD_H_

#include "simd.hpp"

namespace core {
namespace math {

/** ---- simd load/store functions --------------------------------------------
 * simd_load
 * @brief Load 128-bits (2 packed double-precision 64-bit)
 * from the specified row in the 2d-matrix (16-byte aligned).
 *
 * @fn _mm256_extractf128_pd(__m256d a, const int mask)
 * CASE mask[7:0] of
 *  0: dst[127:0] := a[127:0]
 *  1: dst[127:0] := a[255:128]
 * ESAC
 */
core_inline_
__m128d simd_load(const mat2<double> &mat, const size_t row)
{
    return _mm_load_pd(mat.data() + row*mat.dim());
}

/**
 * simd_store
 * @brief Store 128-bits (2 packed double-precision 64-bit)
 * into the specified row in the 2d-matrix (16-byte aligned).
 *
 * @fn _mm256_insertf128_pd(__m256d a, __m128d b, int mask)
 * dst[255:0] := a[255:0]
 * CASE mask[7:0] of
 *  0: dst[127:0]   := b[127:0]
 *  1: dst[255:128] := b[127:0]
 * ESAC
 */
core_inline_
void simd_store(mat2<double> &mat, const size_t row, const __m128d a)
{
    _mm_store_pd(mat.data() + row*mat.dim(), a);
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic matrix operators.
 */
template<>
core_inline_
mat2<double> &mat2<double>::operator+=(const mat2<double> &other)
{
    const __m128d a0_ = simd_load(*this, 0);
    const __m128d a1_ = simd_load(*this, 1);

    const __m128d b0_ = simd_load(other, 0);
    const __m128d b1_ = simd_load(other, 1);

    simd_store(*this, 0, _mm_add_pd(a0_, b0_));
    simd_store(*this, 1, _mm_add_pd(a1_, b1_));
    return *this;
}

template<>
core_inline_
mat2<double> &mat2<double>::operator-=(const mat2<double> &other)
{
    const __m128d a0_ = simd_load(*this, 0);
    const __m128d a1_ = simd_load(*this, 1);

    const __m128d b0_ = simd_load(other, 0);
    const __m128d b1_ = simd_load(other, 1);

    simd_store(*this, 0, _mm_sub_pd(a0_, b0_));
    simd_store(*this, 1, _mm_sub_pd(a1_, b1_));
    return *this;
}

template<>
core_inline_
mat2<double> &mat2<double>::operator*=(const mat2<double> &other)
{
    const __m128d a0_ = simd_load(*this, 0);
    const __m128d a1_ = simd_load(*this, 1);

    const __m128d b0_ = simd_load(other, 0);
    const __m128d b1_ = simd_load(other, 1);

    simd_store(*this, 0, _mm_mul_pd(a0_, b0_));
    simd_store(*this, 1, _mm_mul_pd(a1_, b1_));
    return *this;
}

template<>
core_inline_
mat2<double> &mat2<double>::operator/=(const mat2<double> &other)
{
    const __m128d a0_ = simd_load(*this, 0);
    const __m128d a1_ = simd_load(*this, 1);

    const __m128d b0_ = simd_load(other, 0);
    const __m128d b1_ = simd_load(other, 1);

    simd_store(*this, 0, _mm_div_pd(a0_, b0_));
    simd_store(*this, 1, _mm_div_pd(a1_, b1_));
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<>
core_inline_
mat2<double> &mat2<double>::operator+=(const double scalar)
{
    const __m128d a0_ = simd_load(*this, 0);
    const __m128d a1_ = simd_load(*this, 1);
    const __m128d b_ = _mm_set1_pd(scalar);

    simd_store(*this, 0, _mm_add_pd(a0_, b_));
    simd_store(*this, 1, _mm_add_pd(a1_, b_));
    return *this;
}

template<>
core_inline_
mat2<double> &mat2<double>::operator-=(const double scalar)
{
    const __m128d a0_ = simd_load(*this, 0);
    const __m128d a1_ = simd_load(*this, 1);
    const __m128d b_ = _mm_set1_pd(scalar);

    simd_store(*this, 0, _mm_sub_pd(a0_, b_));
    simd_store(*this, 1, _mm_sub_pd(a1_, b_));
    return *this;
}

template<>
core_inline_
mat2<double> &mat2<double>::operator*=(const double scalar)
{
    const __m128d a0_ = simd_load(*this, 0);
    const __m128d a1_ = simd_load(*this, 1);
    const __m128d b_ = _mm_set1_pd(scalar);

    simd_store(*this, 0, _mm_mul_pd(a0_, b_));
    simd_store(*this, 1, _mm_mul_pd(a1_, b_));
    return *this;
}

template<>
core_inline_
mat2<double> &mat2<double>::operator/=(const double scalar)
{
    const __m128d a0_ = simd_load(*this, 0);
    const __m128d a1_ = simd_load(*this, 1);
    const __m128d b_ = _mm_set1_pd(scalar);

    simd_store(*this, 0, _mm_div_pd(a0_, b_));
    simd_store(*this, 1, _mm_div_pd(a1_, b_));
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

#endif /* CORE_GEOMETRY_MAT2_SIMD_H_ */
