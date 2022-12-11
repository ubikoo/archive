/*
 * transform-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_TRANSFORM_SIMD_H_
#define CORE_GEOMETRY_TRANSFORM_SIMD_H_

#include "simd.hpp"

namespace core {
namespace math {

/** ---------------------------------------------------------------------------
 * rotate
 * @brief Rotate this matrix by theta given an axis of rotation defined by n.
 */
template<>
core_inline_
mat4<double> rotate(vec3<double> n, const double theta)
{
    /* Identity matrix */
    const __m256d one0_ = _mm256_set_pd(0.0, 0.0, 0.0, 1.0);
    const __m256d one1_ = _mm256_set_pd(0.0, 0.0, 1.0, 0.0);
    const __m256d one2_ = _mm256_set_pd(0.0, 1.0, 0.0, 0.0);
    const __m256d one3_ = _mm256_set_pd(1.0, 0.0, 0.0, 0.0);

    /* Compute trigonometric coefficients */
    __m256d alpha_ = _mm256_set1_pd(1.0 - std::cos(theta));
    __m256d beta_  = _mm256_set1_pd(std::cos(theta));
    __m256d gamma_ = _mm256_set1_pd(std::sin(theta));

    /* Compute diadic product matrix */
    n = normalize(n);
    __m256d nn_  = simd_load(n);
    __m256d nn0_ = _mm256_set1_pd(n(0));
    __m256d nn1_ = _mm256_set1_pd(n(1));
    __m256d nn2_ = _mm256_set1_pd(n(2));
    __m256d nn3_ = one3_;

    nn0_ = _mm256_mul_pd(alpha_, _mm256_mul_pd(nn0_, nn_));
    nn1_ = _mm256_mul_pd(alpha_, _mm256_mul_pd(nn1_, nn_));
    nn2_ = _mm256_mul_pd(alpha_, _mm256_mul_pd(nn2_, nn_));
    nn3_ = _mm256_mul_pd(alpha_, nn3_);

    /* Compute identity matrix */
    __m256d id0_ = _mm256_mul_pd(beta_, one0_);
    __m256d id1_ = _mm256_mul_pd(beta_, one1_);
    __m256d id2_ = _mm256_mul_pd(beta_, one2_);
    __m256d id3_ = _mm256_mul_pd(beta_, one3_);

    /* Compute cross product matrix */
    __m256d rc0_ = _mm256_set_pd(0.0,  n(1), -n(2),  0.0);
    __m256d rc1_ = _mm256_set_pd(0.0, -n(0),   0.0,  n(2));
    __m256d rc2_ = _mm256_set_pd(0.0,   0.0,  n(0), -n(1));
    __m256d rc3_ = _mm256_set_pd(1.0,   0.0,   0.0,  1.0);

    rc0_ = _mm256_mul_pd(gamma_, rc0_);
    rc1_ = _mm256_mul_pd(gamma_, rc1_);
    rc2_ = _mm256_mul_pd(gamma_, rc2_);
    rc3_ = _mm256_mul_pd(gamma_, rc3_);

    /* Compute rotation matrix */
    __m256d rot0_ = _mm256_add_pd(nn0_, _mm256_add_pd(id0_, rc0_));
    __m256d rot1_ = _mm256_add_pd(nn1_, _mm256_add_pd(id1_, rc1_));
    __m256d rot2_ = _mm256_add_pd(nn2_, _mm256_add_pd(id2_, rc2_));
    __m256d rot3_ = _mm256_add_pd(nn3_, _mm256_add_pd(id3_, rc3_));

    mat4<double> result;
    simd_store(result, 0, rot0_);
    simd_store(result, 1, rot1_);
    simd_store(result, 2, rot2_);
    simd_store(result, 3, rot3_);
    return result;
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

#endif /* CORE_GEOMETRY_TRANSFORM_SIMD_H_ */
