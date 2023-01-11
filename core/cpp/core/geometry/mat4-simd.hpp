/*
 * mat4-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_MAT4_SIMD_H_
#define CORE_GEOMETRY_MAT4_SIMD_H_

#include "simd.hpp"

namespace core {
namespace math {

/** ---- simd load/store functions --------------------------------------------
 * simd_load
 * @brief Load 256-bits (4 packed double precision 64-bit)
 * from the specified row in the 4d-matrix (32-byte aligned).
 */
core_inline_
__m256d simd_load(const mat4<double> &mat, const size_t row)
{
    return _mm256_load_pd(mat.data() + row*mat.dim());
}

/**
 * simd_store
 * @brief Store 256-bits (4 packed double precision 64-bit)
 * into the specified row in the 4d-matrix (32-byte aligned).
 */
core_inline_
void simd_store(mat4<double> &mat, const size_t row, const __m256d a)
{
    _mm256_store_pd(mat.data() + row*mat.dim(), a);
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic matrix operators.
 */
template<>
core_inline_
mat4<double> &mat4<double>::operator+=(const mat4<double> &other)
{
    const __m256d a0_ = simd_load(*this, 0);
    const __m256d a1_ = simd_load(*this, 1);
    const __m256d a2_ = simd_load(*this, 2);
    const __m256d a3_ = simd_load(*this, 3);

    const __m256d b0_ = simd_load(other, 0);
    const __m256d b1_ = simd_load(other, 1);
    const __m256d b2_ = simd_load(other, 2);
    const __m256d b3_ = simd_load(other, 3);

    simd_store(*this, 0, _mm256_add_pd(a0_, b0_));
    simd_store(*this, 1, _mm256_add_pd(a1_, b1_));
    simd_store(*this, 2, _mm256_add_pd(a2_, b2_));
    simd_store(*this, 3, _mm256_add_pd(a3_, b3_));
    return *this;
}

template<>
core_inline_
mat4<double> &mat4<double>::operator-=(const mat4<double> &other)
{
    const __m256d a0_ = simd_load(*this, 0);
    const __m256d a1_ = simd_load(*this, 1);
    const __m256d a2_ = simd_load(*this, 2);
    const __m256d a3_ = simd_load(*this, 3);

    const __m256d b0_ = simd_load(other, 0);
    const __m256d b1_ = simd_load(other, 1);
    const __m256d b2_ = simd_load(other, 2);
    const __m256d b3_ = simd_load(other, 3);

    simd_store(*this, 0, _mm256_sub_pd(a0_, b0_));
    simd_store(*this, 1, _mm256_sub_pd(a1_, b1_));
    simd_store(*this, 2, _mm256_sub_pd(a2_, b2_));
    simd_store(*this, 3, _mm256_sub_pd(a3_, b3_));
    return *this;
}

template<>
core_inline_
mat4<double> &mat4<double>::operator*=(const mat4<double> &other)
{
    const __m256d a0_ = simd_load(*this, 0);
    const __m256d a1_ = simd_load(*this, 1);
    const __m256d a2_ = simd_load(*this, 2);
    const __m256d a3_ = simd_load(*this, 3);

    const __m256d b0_ = simd_load(other, 0);
    const __m256d b1_ = simd_load(other, 1);
    const __m256d b2_ = simd_load(other, 2);
    const __m256d b3_ = simd_load(other, 3);

    simd_store(*this, 0, _mm256_mul_pd(a0_, b0_));
    simd_store(*this, 1, _mm256_mul_pd(a1_, b1_));
    simd_store(*this, 2, _mm256_mul_pd(a2_, b2_));
    simd_store(*this, 3, _mm256_mul_pd(a3_, b3_));
    return *this;
}

template<>
core_inline_
mat4<double> &mat4<double>::operator/=(const mat4<double> &other)
{
    const __m256d a0_ = simd_load(*this, 0);
    const __m256d a1_ = simd_load(*this, 1);
    const __m256d a2_ = simd_load(*this, 2);
    const __m256d a3_ = simd_load(*this, 3);

    const __m256d b0_ = simd_load(other, 0);
    const __m256d b1_ = simd_load(other, 1);
    const __m256d b2_ = simd_load(other, 2);
    const __m256d b3_ = simd_load(other, 3);

    simd_store(*this, 0, _mm256_div_pd(a0_, b0_));
    simd_store(*this, 1, _mm256_div_pd(a1_, b1_));
    simd_store(*this, 2, _mm256_div_pd(a2_, b2_));
    simd_store(*this, 3, _mm256_div_pd(a3_, b3_));
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<>
core_inline_
mat4<double> &mat4<double>::operator+=(const double scalar)
{
    const __m256d a0_ = simd_load(*this, 0);
    const __m256d a1_ = simd_load(*this, 1);
    const __m256d a2_ = simd_load(*this, 2);
    const __m256d a3_ = simd_load(*this, 3);
    const __m256d b_ = _mm256_set1_pd(scalar);

    simd_store(*this, 0, _mm256_add_pd(a0_, b_));
    simd_store(*this, 1, _mm256_add_pd(a1_, b_));
    simd_store(*this, 2, _mm256_add_pd(a2_, b_));
    simd_store(*this, 3, _mm256_add_pd(a3_, b_));
    return *this;
}

template<>
core_inline_
mat4<double> &mat4<double>::operator-=(const double scalar)
{
    const __m256d a0_ = simd_load(*this, 0);
    const __m256d a1_ = simd_load(*this, 1);
    const __m256d a2_ = simd_load(*this, 2);
    const __m256d a3_ = simd_load(*this, 3);
    const __m256d b_ = _mm256_set1_pd(scalar);

    simd_store(*this, 0, _mm256_sub_pd(a0_, b_));
    simd_store(*this, 1, _mm256_sub_pd(a1_, b_));
    simd_store(*this, 2, _mm256_sub_pd(a2_, b_));
    simd_store(*this, 3, _mm256_sub_pd(a3_, b_));
    return *this;
}

template<>
core_inline_
mat4<double> &mat4<double>::operator*=(const double scalar)
{
    const __m256d a0_ = simd_load(*this, 0);
    const __m256d a1_ = simd_load(*this, 1);
    const __m256d a2_ = simd_load(*this, 2);
    const __m256d a3_ = simd_load(*this, 3);
    const __m256d b_ = _mm256_set1_pd(scalar);

    simd_store(*this, 0, _mm256_mul_pd(a0_, b_));
    simd_store(*this, 1, _mm256_mul_pd(a1_, b_));
    simd_store(*this, 2, _mm256_mul_pd(a2_, b_));
    simd_store(*this, 3, _mm256_mul_pd(a3_, b_));
    return *this;
}

template<>
core_inline_
mat4<double> &mat4<double>::operator/=(const double scalar)
{
    const __m256d a0_ = simd_load(*this, 0);
    const __m256d a1_ = simd_load(*this, 1);
    const __m256d a2_ = simd_load(*this, 2);
    const __m256d a3_ = simd_load(*this, 3);
    const __m256d b_ = _mm256_set1_pd(scalar);

    simd_store(*this, 0, _mm256_div_pd(a0_, b_));
    simd_store(*this, 1, _mm256_div_pd(a1_, b_));
    simd_store(*this, 2, _mm256_div_pd(a2_, b_));
    simd_store(*this, 3, _mm256_div_pd(a3_, b_));
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

#endif /* CORE_GEOMETRY_MAT4_SIMD_H_ */
