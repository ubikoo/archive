/*
 * algebra-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_ALGEBRA_SIMD_H_
#define CORE_GEOMETRY_ALGEBRA_SIMD_H_

#include "simd.hpp"

namespace core {
namespace math {

/** ---------------------------------------------------------------------------
 * dot
 * @brief Return the dot product of two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/dot.xhtml
 */
template<>
core_inline_
double dot(const vec2<double> &a, const vec2<double> &b)
{
    const __m128d a_ = simd_load(a);
    const __m128d b_ = simd_load(b);
    return _mm_cvtsd_f64(simd128_dot_(a_, b_));
}

template<>
core_inline_
double dot(const vec3<double> &a, const vec3<double> &b)
{
    const __m256d a_ = simd_load(a);
    const __m256d b_ = simd_load(b);
    return _mm256_cvtsd_f64(simd256_dot_(a_, b_));
}

template<>
core_inline_
double dot(const vec4<double> &a, const vec4<double> &b)
{
    const __m256d a_ = simd_load(a);
    const __m256d b_ = simd_load(b);
    return _mm256_cvtsd_f64(simd256_dot_(a_, b_));
}


/** ---------------------------------------------------------------------------
 * norm
 * @brief Return the norm product of two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/length.xhtml
 */
template<>
core_inline_
double norm(const vec2<double> &a)
{
    const __m128d a_ = simd_load(a);
    return _mm_cvtsd_f64(simd128_norm_(a_));
}

template<>
core_inline_
double norm(const vec3<double> &a)
{
    const __m256d a_ = simd_load(a);
    return _mm256_cvtsd_f64(simd256_norm_(a_));
}

template<>
core_inline_
double norm(const vec4<double> &a)
{
    const __m256d a_ = simd_load(a);
    return _mm256_cvtsd_f64(simd256_norm_(a_));
}


/** ---------------------------------------------------------------------------
 * normalize
 * @brief Return the normalize product of two vectors.
 * @see
 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/normalize.xhtml
 */
template<>
core_inline_
vec2<double> normalize(const vec2<double> &a)
{
    const __m128d a_ = simd_load(a);
    vec2<double> result;
    simd_store(result, simd128_normalize_(a_));
    return result;
}

template<>
core_inline_
vec3<double> normalize(const vec3<double> &a)
{
    const __m256d a_ = simd_load(a);
    vec3<double> result;
    simd_store(result, simd256_normalize_(a_));
    return result;
}

template<>
core_inline_
vec4<double> normalize(const vec4<double> &a)
{
    const __m256d a_ = simd_load(a);
    vec4<double> result;
    simd_store(result, simd256_normalize_(a_));
    return result;
}


/** ---------------------------------------------------------------------------
 * distance
 * @brief Return the distance product of two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/distance.xhtml
 */
template<>
core_inline_
double distance(const vec2<double> &a, const vec2<double> &b)
{
    return norm(a-b);
}

template<>
core_inline_
double distance(const vec3<double> &a, const vec3<double> &b)
{
    return norm(a-b);
}

template<>
core_inline_
double distance(const vec4<double> &a, const vec4<double> &b)
{
    return norm(a-b);
}


/** ---------------------------------------------------------------------------
 * cross
 * @brief Return the cross product of two vectors.
 *
 * c = a x b
 *   = {a1*b2 - a2*b1,
 *      a2*b0 - a0*b2,
 *      a0*b1 - a1*b0}
 *   = {a1*b2, a2*b0, a0*b1} -
 *     {b1*a2, b2*a0, b0*a1}
 *   = {a1, a2, a0} * {b2, b0, b1} -
 *     {b1, b2, b0} * {a2, a0, a1}
 *   = {0, a0, a2, a1} * {0, b1, b0, b2} -
 *     {0, b0, b2, b1} * {0, a1, a0, a2}
 *
 * The cross product member function is interpreted as a projection
 * operation of this vector on the rhs vector.
 * The this vector acts as a projection matrix rotating the rhs vector
 * in a direction mutually perpendicular to the original vectors,
 *
 *     |  0 -a2   a1| |b0|
 * c = | a2   0  -a0|*|b1|
 *     |-a1  a0    0| |b2|
 *   = {a1*b2 - a2*b1,
 *      a2*b0 - a0*b2,
 *      a0*b1 - a1*b0}
 *
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/cross.xhtml
 */
template<>
core_inline_
vec3<double> cross(const vec3<double> &a, const vec3<double> &b)
{
    /*
     * a0_ = { 0, a2, a1, a0}
     * a1_ = {a1, a0,  0, a2}
     * a2_ = {a0,  0, a2, a1}
     * a3_ = { 0, a0, a2, a1}(*)
     * a4_ = {a1,  0, a2, a0}
     * a5_ = { 0, a1, a0, a2}(*)
     */
    __m256d a0_ = simd_load(a);
    __m256d a1_ = _mm256_permute2f128_pd(a0_, a0_, core_extension_(0b00000001));
    __m256d a2_ = _mm256_shuffle_pd(a0_, a1_, core_extension_(0b0101));
    __m256d a3_ = _mm256_permute_pd(a2_, core_extension_(0b0110));
    __m256d a4_ = _mm256_shuffle_pd(a0_, a1_, core_extension_(0b1100));
    __m256d a5_ = _mm256_permute_pd(a4_, core_extension_(0b0101));
    /*
     * b0_ = { 0, b2, b1, b0}
     * b1_ = {b1, b0,  0, b2}
     * b2_ = {b0,  0, b2, b1}
     * b3_ = { 0, b0, b2, b1} (*)
     * b4_ = {b1,  0, b2, b0}
     * b5_ = { 0, b1, b0, b2} (*)
     */
    __m256d b0_ = simd_load(b);
    __m256d b1_ = _mm256_permute2f128_pd(b0_, b0_, core_extension_(0b00000001));
    __m256d b2_ = _mm256_shuffle_pd(b0_, b1_, core_extension_(0b0101));
    __m256d b3_ = _mm256_permute_pd(b2_, core_extension_(0b0110));
    __m256d b4_ = _mm256_shuffle_pd(b0_, b1_, core_extension_(0b1100));
    __m256d b5_ = _mm256_permute_pd(b4_, core_extension_(0b0101));
    /*
     * c_ = (a3_ * b5_) - (a5_ * b3_)
     */
    vec3<double> result;
    simd_store(
        result,
        _mm256_sub_pd(
            _mm256_mul_pd(a3_, b5_),
            _mm256_mul_pd(a5_, b3_)));
    return result;
}


/** ---------------------------------------------------------------------------
 * transpose
 * @brief Return the transpose matrix
 */
template<>
core_inline_
mat2<double> transpose(const mat2<double> &a)
{
    /*
     * r0_ = {a1, a0} -> {a2, a0}
     * r1_ = {a3, a2} -> {a3, a1}
     */
    __m128d row_[2];
    row_[0] = simd_load(a, 0);
    row_[1] = simd_load(a, 1);
    simd128_transpose_(row_);

    mat2<double> result;
    simd_store(result, 0, row_[0]);
    simd_store(result, 1, row_[1]);
    return result;
}

template<>
core_inline_
mat3<double> transpose(const mat3<double> &a)
{
    /*
     * r0_ = { 0,  a2,  a1,  a0} -> { 0,  a8,  a4,  a0}
     * r1_ = { 0,  a6,  a5,  a4} -> { 0,  a9,  a5,  a1}
     * r2_ = { 0, a10,  a9,  a8} -> { 0, a10,  a6,  a2}
     * r3_ = { 0,   0,   0,   0} -> { 0,   0,   0,   0}
     */
    __m256d row_[4];
    row_[0] = simd_load(a, 0);
    row_[1] = simd_load(a, 1);
    row_[2] = simd_load(a, 2);
    row_[3] = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);
    simd256_transpose_(row_);

    mat3<double> result;
    simd_store(result, 0, row_[0]);
    simd_store(result, 1, row_[1]);
    simd_store(result, 2, row_[2]);
    return result;
}

template<>
core_inline_
mat4<double> transpose(const mat4<double> &a)
{
    /*
     * r0_ = { a3,  a2,  a1,  a0} -> { a12,  a8,  a4,  a0}
     * r1_ = { a7,  a6,  a5,  a4} -> { a13,  a9,  a5,  a1}
     * r2_ = {a11, a10,  a9,  a8} -> { a14, a10,  a6,  a2}
     * r3_ = {a15, a14, a13, a12} -> { a15, a11,  a7,  a3}
     */
    __m256d row_[4];
    row_[0] = simd_load(a, 0);
    row_[1] = simd_load(a, 1);
    row_[2] = simd_load(a, 2);
    row_[3] = simd_load(a, 3);
    simd256_transpose_(row_);

    mat4<double> result;
    simd_store(result, 0, row_[0]);
    simd_store(result, 1, row_[1]);
    simd_store(result, 2, row_[2]);
    simd_store(result, 3, row_[3]);
    return result;
}


/** ---------------------------------------------------------------------------
 * determinant
 *
 * Compute the determinant of the specified matrix and return
 * the lower double-precision (64-bit) floating-point element.
 */
template<>
core_inline_
double determinant(const mat2<double> &a)
{
    /*
     * Compute the determinant of a 2x2 matrix as a packed vector:
     *
     * det(a) = det{a0, a1,
     *              a2, a3}
     */
    __m128d m0_ = simd_load(a, 0);
    __m128d m1_ = simd_load(a, 1);
    __m256d det_ = simd256_det_(_mm256_set_m128d(m1_, m0_));

    return _mm256_cvtsd_f64(det_);
}

template<>
core_inline_
double determinant(const mat3<double> &a)
{
    /*
     * Compute the determinant of a 3x3 matrix as a packed vector:
     *
     * det(a) = det{a0, a1, a2,
     *              a3, a4, a5,
     *              a6, a7, a8}
     */
    __m256d a0_ = _mm256_set1_pd(a(0,0));
    __m256d a1_ = _mm256_set1_pd(a(0,1));
    __m256d a2_ = _mm256_set1_pd(a(0,2));
    /*
     * Compute the (i,j) minors of matrix a
     *
     * m0_ = det{a8, a7, a5, a4}
     * m1_ = det{a6, a8, a3, a5}
     * m2_ = det{a7, a6, a4, a3}
     */
    __m256d m0_ = simd256_det_(_mm256_set_pd(a(2,2), a(2,1), a(1,2), a(1,1)));
    __m256d m1_ = simd256_det_(_mm256_set_pd(a(2,0), a(2,2), a(1,0), a(1,2)));
    __m256d m2_ = simd256_det_(_mm256_set_pd(a(2,1), a(2,0), a(1,1), a(1,0)));
    /*
     * det(a) =  a0 * det{a8, a7, a5, a4}
     *          +a1 * det{a6, a8, a3, a5}
     *          +a2 * det{a7, a6, a4, a3}
     *        = a0_ * m0_ +
     *          a1_ * m1_ +
     *          a2_ * m2_
     */
    __m256d det_ = _mm256_set1_pd(0.0);
    det_ = _mm256_add_pd(det_, _mm256_mul_pd(a0_, m0_));
    det_ = _mm256_add_pd(det_, _mm256_mul_pd(a1_, m1_));
    det_ = _mm256_add_pd(det_, _mm256_mul_pd(a2_, m2_));

    return _mm256_cvtsd_f64(det_);
}

template<>
core_inline_
double determinant(const mat4<double> &a)
{
    /*
     * Compute the determinant of a 4x4 matrix as a packed vector:
     *
     * det(a) = det{a0,  a1,  a2,  a3,
     *              a4,  a5,  a6,  a7,
     *              a8,  a9,  a10, a11,
     *              a12, a13, a14, a15}
     *
     * Compute the (i,j) minors of a
     *
     * m0_  = det{a5,  a4,  a1,  a0}
     * m1_  = det{a15, a14, a11, a10}
     *
     * m2_  = det{a4,  a6,  a0,  a2}
     * m3_  = det{a15, a13, a11, a9}
     *
     * m4_  = det{a7,  a4,  a3,  a0}
     * m5_  = det{a14, a13, a10, a9}
     *
     * m6_  = det{a6,  a5,  a2,  a1}
     * m7_  = det{a15, a12, a11, a8}
     *
     * m8_  = det{a5,  a7,  a1,  a3}
     * m9_  = det{a14, a12, a10, a8}
     *
     * m10_ = det{a7,  a6,  a3, a2}
     * m11_ = det{a13, a12, a9, a8}
     */
    __m256d m0_  = simd256_det_(_mm256_set_pd(a(1,1), a(1,0), a(0,1), a(0,0)));
    __m256d m1_  = simd256_det_(_mm256_set_pd(a(3,3), a(3,2), a(2,3), a(2,2)));

    __m256d m2_  = simd256_det_(_mm256_set_pd(a(1,0), a(1,2), a(0,0), a(0,2)));
    __m256d m3_  = simd256_det_(_mm256_set_pd(a(3,3), a(3,1), a(2,3), a(2,1)));

    __m256d m4_  = simd256_det_(_mm256_set_pd(a(1,3), a(1,0), a(0,3), a(0,0)));
    __m256d m5_  = simd256_det_(_mm256_set_pd(a(3,2), a(3,1), a(2,2), a(2,1)));

    __m256d m6_  = simd256_det_(_mm256_set_pd(a(1,2), a(1,1), a(0,2), a(0,1)));
    __m256d m7_  = simd256_det_(_mm256_set_pd(a(3,3), a(3,0), a(2,3), a(2,0)));

    __m256d m8_  = simd256_det_(_mm256_set_pd(a(1,1), a(1,3), a(0,1), a(0,3)));
    __m256d m9_  = simd256_det_(_mm256_set_pd(a(3,2), a(3,0), a(2,2), a(2,0)));

    __m256d m10_ = simd256_det_(_mm256_set_pd(a(1,3), a(1,2), a(0,3), a(0,2)));
    __m256d m11_ = simd256_det_(_mm256_set_pd(a(3,1), a(3,0), a(2,1), a(2,0)));
    /*
     * det(a) = m0_  * m1_ +
     *          m2_  * m3_ +
     *          m4_  * m5_ +
     *          m6_  * m7_ +
     *          m8_  * m9_ +
     *          m10_ * m11_
     */
    __m256d det_ = _mm256_set1_pd(0.0);
    det_ = _mm256_add_pd(det_, _mm256_mul_pd(m0_,  m1_));
    det_ = _mm256_add_pd(det_, _mm256_mul_pd(m2_,  m3_));
    det_ = _mm256_add_pd(det_, _mm256_mul_pd(m4_,  m5_));
    det_ = _mm256_add_pd(det_, _mm256_mul_pd(m6_,  m7_));
    det_ = _mm256_add_pd(det_, _mm256_mul_pd(m8_,  m9_));
    det_ = _mm256_add_pd(det_, _mm256_mul_pd(m10_, m11_));

    return _mm256_cvtsd_f64(det_);
}


/** ---------------------------------------------------------------------------
 * inverse
 * @brief Return the inverse of the matrix.
 */
template<>
core_inline_
mat2<double> inverse(const mat2<double> &a)
{
    /*
     * Compute the inverse of matrix:
     *  a = {a0, a1,
     *       a2, a3}
     *
     * from its adjugate and corresponding determinant:
     *  inv(a) = adj(a) / det(a)
     *
     * where adj(a) = C(a)^t is given in terms of the cofactor matrix
     * and corresponding minors:
     *  C(a) = (-1)^(i+j)M_ij
     */
    const __m256d zero_ = _mm256_set1_pd(0.0);
    const __m256d one_  = _mm256_set1_pd(1.0);

    __m256d adj_  = _mm256_set_pd(
         a(0,0), -a(1,0),
        -a(0,1),  a(1,1));
    __m256d det_  = _mm256_div_pd(one_, _mm256_set1_pd(determinant(a)));
    __m256d inv_  = _mm256_mul_pd(adj_, det_);

    /*
     * Check for NaN and set the inverse to zero if the determinant is null.
     */
    __m256d mask_ = _mm256_or_pd(_mm256_cmplt_pd(det_, zero_),
                                 _mm256_cmpgt_pd(det_, zero_));
    inv_ = _mm256_and_pd(inv_, mask_);

    mat2<double> result;
    simd_store(result, 0, _mm256_extractf128_pd(inv_, 0));
    simd_store(result, 1, _mm256_extractf128_pd(inv_, 1));
    return result;
}

template<>
core_inline_
mat3<double> inverse(const mat3<double> &a)
{
    /*
     * Compute the inverse of matrix:
     *  a = {a0, a1, a2,
     *       a3, a4, a5,
     *       a6, a7, a8}
     *
     * from its adjugate and corresponding determinant:
     *  inv(a) = adj(a) / det(a)
     *
     * where adj(a) = C(a)^t is given in terms of the cofactor matrix
     * and corresponding minors:
     *  C(a) = (-1)^(i+j)M_ij
     *
     * m0_  = det{a4, a5, a7, a8}
     * m1_  = det{a5, a3, a8, a6}
     * m2_  = det{a3, a4, a6, a7}
     *
     * m3_  = det{a2, a1, a8, a7}
     * m4_  = det{a0, a2, a6, a8}
     * m5_  = det{a1, a0, a7, a6}
     *
     * m6_  = det{a1, a2, a4, a5}
     * m7_  = det{a2, a0, a5, a3}
     * m8_  = det{a0, a1, a3, a4}
     *
     * adj(a) = {m0_, m3_, m6_,
     *           m1_, m4_, m7_,
     *           m2_, m5_, m8_}
     */
    __m256d m0_ = simd256_det_(_mm256_set_pd(a(1,1), a(1,2), a(2,1), a(2,2)));
    __m256d m1_ = simd256_det_(_mm256_set_pd(a(1,2), a(1,0), a(2,2), a(2,0)));
    __m256d m2_ = simd256_det_(_mm256_set_pd(a(1,0), a(1,1), a(2,0), a(2,1)));
    __m256d m3_ = simd256_det_(_mm256_set_pd(a(0,2), a(0,1), a(2,2), a(2,1)));
    __m256d m4_ = simd256_det_(_mm256_set_pd(a(0,0), a(0,2), a(2,0), a(2,2)));
    __m256d m5_ = simd256_det_(_mm256_set_pd(a(0,1), a(0,0), a(2,1), a(2,0)));
    __m256d m6_ = simd256_det_(_mm256_set_pd(a(0,1), a(0,2), a(1,1), a(1,2)));
    __m256d m7_ = simd256_det_(_mm256_set_pd(a(0,2), a(0,0), a(1,2), a(1,0)));
    __m256d m8_ = simd256_det_(_mm256_set_pd(a(0,0), a(0,1), a(1,0), a(1,1)));
    /*
     * _mm256_unpackhi_pd(__m256d a, __m256d b)
     * dst[63:0]    := a[127:64]
     * dst[127:64]  := b[127:64]
     * dst[191:128] := a[255:192]
     * dst[255:192] := b[255:192]
     *
     * _mm256_permute2f128_pd(__m256d a, __m256d b, int mask)
     * SELECT4(src1, src2, control) {
     *  CASE(control[1:0])
     *      0:  tmp[127:0] := src1[127:0]
     *      1:  tmp[127:0] := src1[255:128]
     *      2:  tmp[127:0] := src2[127:0]
     *      3:  tmp[127:0] := src2[255:128]
     *  ESAC
     *  IF control[3]
     *      tmp[127:0] := 0
     *  FI
     *  RETURN tmp[127:0]
     * }
     * dst[127:0]   := SELECT4(a[255:0], b[255:0], mask[3:0])
     * dst[255:128] := SELECT4(a[255:0], b[255:0], mask[7:4])
     */
    const __m256d zero_ = _mm256_set1_pd(0.0);
    const __m256d one_  = _mm256_set1_pd(1.0);
    __m256d det_ = _mm256_div_pd(one_, _mm256_set1_pd(determinant(a)));

    /*
     * inv0_ = {0, m6_, m3_, m0_}
     */
    __m256d inv0_ = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(m0_, m3_),
        _mm256_unpackhi_pd(m6_, zero_),
        core_extension_(0b00100000));
    inv0_ = _mm256_mul_pd(inv0_, det_);

    /*
     * inv1_ = {0, m7_, m4_, m1_}
     */
    __m256d inv1_ = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(m1_, m4_),
        _mm256_unpackhi_pd(m7_, zero_),
        core_extension_(0b00100000));
    inv1_ = _mm256_mul_pd(inv1_, det_);

    /*
     * inv2_ = {0, m8_, m5_, m2_}
     */
    __m256d inv2_ = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(m2_, m5_),
        _mm256_unpackhi_pd(m8_, zero_),
        core_extension_(0b00100000));
    inv2_ = _mm256_mul_pd(inv2_, det_);

    /*
     * Check for NaN and set the inverse to zero if the determinant is null.
     */
    __m256d mask_ = _mm256_or_pd(
        _mm256_cmplt_pd(det_, zero_),
        _mm256_cmpgt_pd(det_, zero_));
    inv0_ = _mm256_and_pd(inv0_, mask_);
    inv1_ = _mm256_and_pd(inv1_, mask_);
    inv2_ = _mm256_and_pd(inv2_, mask_);

    mat3<double> result;
    simd_store(result, 0, inv0_);
    simd_store(result, 1, inv1_);
    simd_store(result, 2, inv2_);
    return result;
}

template<>
core_inline_
mat4<double> inverse(const mat4<double> &a)
{
    /*
     * Compute the inverse of matrix:
     *  a = {a0,  a1,  a2,  a3,
     *       a4,  a5,  a6,  a7,
     *       a8,  a9,  a10, a11,
     *       a12, a13, a14, a15}
     *
     * from its adjugate and corresponding determinant:
     *  inv(a) = adj(a) / det(a)
     *
     * where adj(a) = C(a)^t is given in terms of the cofactor matrix
     * and corresponding minors:
     *  C(a) = (-1)^(i+j)*M_ij
     *
     * m0_  = det{a10, a11, a14, a15}
     * m1_  = det{a11, a9,  a15, a13}
     * m2_  = det{a9,  a10, a13, a14}
     * m3_  = det{a11, a8,  a15, a12}
     * m4_  = det{a8,  a10, a12, a14}
     * m5_  = det{a8,  a9,  a12, a13}
     *
     * m6_  = det{a2,  a3,  a6,  a7}
     * m7_  = det{a3,  a1,  a7,  a5}
     * m8_  = det{a1,  a2,  a5,  a6}
     * m9_  = det{a3,  a0,  a7,  a4}
     * m10_ = det{a0,  a2,  a4,  a6}
     * m11_ = det{a0,  a1,  a4,  a5}
     *
     * adj0  =  a5  * m0 + a6  * m1  + a7  * m2
     * adj1  = -a1  * m0 - a2  * m1  - a3  * m2
     * adj2  =  a13 * m6 + a14 * m7  + a15 * m8
     * adj3  = -a9  * m6 - a10 * m7  - a11 * m8
     *
     * adj4  = -a4  * m0 - a6  * m3  - a7  * m4
     * adj5  =  a0  * m0 + a2  * m3  + a3  * m4
     * adj6  = -a12 * m6 - a14 * m9  - a15 * m10
     * adj7  =  a8  * m6 + a10 * m9  + a11 * m10
     *
     * adj8  = -a4  * m1 + a5  * m3  + a7  * m5
     * adj9  =  a0  * m1 - a1  * m3  - a3  * m5
     * adj10 = -a12 * m7 + a13 * m9  + a15 * m11
     * adj11 =  a8  * m7 - a9  * m9  - a11 * m11
     *
     * adj12 = -a4  * m2 + a5  * m4  - a6  * m5
     * adj13 =  a0  * m2 - a1  * m4  + a2  * m5
     * adj14 = -a12 * m8 + a13 * m10 - a14 * m11
     * adj15 =  a8  * m8 - a9  * m10 + a10 * m11
     */
    __m256d m0_  = simd256_det_(_mm256_set_pd(a(2,2), a(2,3), a(3,2), a(3,3)));
    __m256d m1_  = simd256_det_(_mm256_set_pd(a(2,3), a(2,1), a(3,3), a(3,1)));

    __m256d m2_  = simd256_det_(_mm256_set_pd(a(2,1), a(2,2), a(3,1), a(3,2)));
    __m256d m3_  = simd256_det_(_mm256_set_pd(a(2,3), a(2,0), a(3,3), a(3,0)));

    __m256d m4_  = simd256_det_(_mm256_set_pd(a(2,0), a(2,2), a(3,0), a(3,2)));
    __m256d m5_  = simd256_det_(_mm256_set_pd(a(2,0), a(2,1), a(3,0), a(3,1)));

    __m256d m6_  = simd256_det_(_mm256_set_pd(a(0,2), a(0,3), a(1,2), a(1,3)));
    __m256d m7_  = simd256_det_(_mm256_set_pd(a(0,3), a(0,1), a(1,3), a(1,1)));

    __m256d m8_  = simd256_det_(_mm256_set_pd(a(0,1), a(0,2), a(1,1), a(1,2)));
    __m256d m9_  = simd256_det_(_mm256_set_pd(a(0,3), a(0,0), a(1,3), a(1,0)));

    __m256d m10_ = simd256_det_(_mm256_set_pd(a(0,0), a(0,2), a(1,0), a(1,2)));
    __m256d m11_ = simd256_det_(_mm256_set_pd(a(0,0), a(0,1), a(1,0), a(1,1)));

    __m256d a0_  = _mm256_set1_pd(a(0,0));
    __m256d a1_  = _mm256_set1_pd(a(0,1));
    __m256d a2_  = _mm256_set1_pd(a(0,2));
    __m256d a3_  = _mm256_set1_pd(a(0,3));

    __m256d a4_  = _mm256_set1_pd(a(1,0));
    __m256d a5_  = _mm256_set1_pd(a(1,1));
    __m256d a6_  = _mm256_set1_pd(a(1,2));
    __m256d a7_  = _mm256_set1_pd(a(1,3));

    __m256d a8_  = _mm256_set1_pd(a(2,0));
    __m256d a9_  = _mm256_set1_pd(a(2,1));
    __m256d a10_ = _mm256_set1_pd(a(2,2));
    __m256d a11_ = _mm256_set1_pd(a(2,3));

    __m256d a12_ = _mm256_set1_pd(a(3,0));
    __m256d a13_ = _mm256_set1_pd(a(3,1));
    __m256d a14_ = _mm256_set1_pd(a(3,2));
    __m256d a15_ = _mm256_set1_pd(a(3,3));

    /*
     * adj0_  =  a5  * m0 + a6  * m1  + a7  * m2
     * adj1_  = -a1  * m0 - a2  * m1  - a3  * m2
     * adj2_  =  a13 * m6 + a14 * m7  + a15 * m8
     * adj3_  = -a9  * m6 - a10 * m7  - a11 * m8
     */
    __m256d adj0_ = _mm256_set1_pd(0.0);
    adj0_ = _mm256_add_pd(adj0_, _mm256_mul_pd(a5_, m0_));
    adj0_ = _mm256_add_pd(adj0_, _mm256_mul_pd(a6_, m1_));
    adj0_ = _mm256_add_pd(adj0_, _mm256_mul_pd(a7_, m2_));

    __m256d adj1_ = _mm256_set1_pd(0.0);
    adj1_ = _mm256_sub_pd(adj1_, _mm256_mul_pd(a1_, m0_));
    adj1_ = _mm256_sub_pd(adj1_, _mm256_mul_pd(a2_, m1_));
    adj1_ = _mm256_sub_pd(adj1_, _mm256_mul_pd(a3_, m2_));

    __m256d adj2_ = _mm256_set1_pd(0.0);
    adj2_ = _mm256_add_pd(adj2_, _mm256_mul_pd(a13_, m6_));
    adj2_ = _mm256_add_pd(adj2_, _mm256_mul_pd(a14_, m7_));
    adj2_ = _mm256_add_pd(adj2_, _mm256_mul_pd(a15_, m8_));

    __m256d adj3_ = _mm256_set1_pd(0.0);
    adj3_ = _mm256_sub_pd(adj3_, _mm256_mul_pd(a9_,  m6_));
    adj3_ = _mm256_sub_pd(adj3_, _mm256_mul_pd(a10_, m7_));
    adj3_ = _mm256_sub_pd(adj3_, _mm256_mul_pd(a11_, m8_));

    /*
     * adj4_  = -a4  * m0 - a6  * m3  - a7  * m4
     * adj5_  =  a0  * m0 + a2  * m3  + a3  * m4
     * adj6_  = -a12 * m6 - a14 * m9  - a15 * m10
     * adj7_  =  a8  * m6 + a10 * m9  + a11 * m10
     */
    __m256d adj4_ = _mm256_set1_pd(0.0);
    adj4_ = _mm256_sub_pd(adj4_, _mm256_mul_pd(a4_, m0_));
    adj4_ = _mm256_sub_pd(adj4_, _mm256_mul_pd(a6_, m3_));
    adj4_ = _mm256_sub_pd(adj4_, _mm256_mul_pd(a7_, m4_));

    __m256d adj5_ = _mm256_set1_pd(0.0);
    adj5_ = _mm256_add_pd(adj5_, _mm256_mul_pd(a0_, m0_));
    adj5_ = _mm256_add_pd(adj5_, _mm256_mul_pd(a2_, m3_));
    adj5_ = _mm256_add_pd(adj5_, _mm256_mul_pd(a3_, m4_));

    __m256d adj6_ = _mm256_set1_pd(0.0);
    adj6_ = _mm256_sub_pd(adj6_, _mm256_mul_pd(a12_, m6_));
    adj6_ = _mm256_sub_pd(adj6_, _mm256_mul_pd(a14_, m9_));
    adj6_ = _mm256_sub_pd(adj6_, _mm256_mul_pd(a15_, m10_));

    __m256d adj7_ = _mm256_set1_pd(0.0);
    adj7_ = _mm256_add_pd(adj7_, _mm256_mul_pd(a8_,  m6_));
    adj7_ = _mm256_add_pd(adj7_, _mm256_mul_pd(a10_, m9_));
    adj7_ = _mm256_add_pd(adj7_, _mm256_mul_pd(a11_, m10_));

    /*
     * adj8_  = -a4  * m1 + a5  * m3  + a7  * m5
     * adj9_  =  a0  * m1 - a1  * m3  - a3  * m5
     * adj10_ = -a12 * m7 + a13 * m9  + a15 * m11
     * adj11_ =  a8  * m7 - a9  * m9  - a11 * m11
     */
    __m256d adj8_ = _mm256_set1_pd(0.0);
    adj8_  = _mm256_sub_pd(adj8_, _mm256_mul_pd(a4_, m1_));
    adj8_  = _mm256_add_pd(adj8_, _mm256_mul_pd(a5_, m3_));
    adj8_  = _mm256_add_pd(adj8_, _mm256_mul_pd(a7_, m5_));

    __m256d adj9_ = _mm256_set1_pd(0.0);
    adj9_  = _mm256_add_pd(adj9_, _mm256_mul_pd(a0_, m1_));
    adj9_  = _mm256_sub_pd(adj9_, _mm256_mul_pd(a1_, m3_));
    adj9_  = _mm256_sub_pd(adj9_, _mm256_mul_pd(a3_, m5_));

    __m256d adj10_ = _mm256_set1_pd(0.0);
    adj10_ = _mm256_sub_pd(adj10_, _mm256_mul_pd(a12_, m7_));
    adj10_ = _mm256_add_pd(adj10_, _mm256_mul_pd(a13_, m9_));
    adj10_ = _mm256_add_pd(adj10_, _mm256_mul_pd(a15_, m11_));

    __m256d adj11_ = _mm256_set1_pd(0.0);
    adj11_ = _mm256_add_pd(adj11_, _mm256_mul_pd(a8_,  m7_));
    adj11_ = _mm256_sub_pd(adj11_, _mm256_mul_pd(a9_,  m9_));
    adj11_ = _mm256_sub_pd(adj11_, _mm256_mul_pd(a11_, m11_));

    /*
     * adj12_ = -a4  * m2 + a5  * m4  - a6  * m5
     * adj13_ =  a0  * m2 - a1  * m4  + a2  * m5
     * adj14_ = -a12 * m8 + a13 * m10 - a14 * m11
     * adj15_ =  a8  * m8 - a9  * m10 + a10 * m11
     */
    __m256d adj12_ = _mm256_set1_pd(0.0);
    adj12_ = _mm256_sub_pd(adj12_, _mm256_mul_pd(a4_, m2_));
    adj12_ = _mm256_add_pd(adj12_, _mm256_mul_pd(a5_, m4_));
    adj12_ = _mm256_sub_pd(adj12_, _mm256_mul_pd(a6_, m5_));

    __m256d adj13_ = _mm256_set1_pd(0.0);
    adj13_ = _mm256_add_pd(adj13_, _mm256_mul_pd(a0_, m2_));
    adj13_ = _mm256_sub_pd(adj13_, _mm256_mul_pd(a1_, m4_));
    adj13_ = _mm256_add_pd(adj13_, _mm256_mul_pd(a2_, m5_));

    __m256d adj14_ = _mm256_set1_pd(0.0);
    adj14_ = _mm256_sub_pd(adj14_, _mm256_mul_pd(a12_, m8_));
    adj14_ = _mm256_add_pd(adj14_, _mm256_mul_pd(a13_, m10_));
    adj14_ = _mm256_sub_pd(adj14_, _mm256_mul_pd(a14_, m11_));

    __m256d adj15_ = _mm256_set1_pd(0.0);
    adj15_ = _mm256_add_pd(adj15_, _mm256_mul_pd(a8_,  m8_));
    adj15_ = _mm256_sub_pd(adj15_, _mm256_mul_pd(a9_,  m10_));
    adj15_ = _mm256_add_pd(adj15_, _mm256_mul_pd(a10_, m11_));

    /*
     * _mm256_unpackhi_pd(__m256d a, __m256d b)
     * dst[63:0]    := a[127:64]
     * dst[127:64]  := b[127:64]
     * dst[191:128] := a[255:192]
     * dst[255:192] := b[255:192]
     *
     * _mm256_permute2f128_pd(__m256d a, __m256d b, int mask)
     * SELECT4(src1, src2, control) {
     *  CASE(control[1:0])
     *      0:  tmp[127:0] := src1[127:0]
     *      1:  tmp[127:0] := src1[255:128]
     *      2:  tmp[127:0] := src2[127:0]
     *      3:  tmp[127:0] := src2[255:128]
     *  ESAC
     *  IF control[3]
     *      tmp[127:0] := 0
     *  FI
     *  RETURN tmp[127:0]
     * }
     * dst[127:0]   := SELECT4(a[255:0], b[255:0], mask[3:0])
     * dst[255:128] := SELECT4(a[255:0], b[255:0], mask[7:4])
     */
    const __m256d zero_ = _mm256_set1_pd(0.0);
    const __m256d one_  = _mm256_set1_pd(1.0);
    __m256d det_ = _mm256_div_pd(one_, _mm256_set1_pd(determinant(a)));

    /*
     * inv0_ = {adj3_, adj2_, adj1_, adj0_}
     */
    __m256d inv0_ = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(adj0_, adj1_),
        _mm256_unpackhi_pd(adj2_, adj3_),
        core_extension_(0b00100000));
    inv0_ = _mm256_mul_pd(inv0_, det_);

    /*
     * inv1_ = {adj7_, adj6_, adj5_, adj4_}
     */
    __m256d inv1_ = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(adj4_, adj5_),
        _mm256_unpackhi_pd(adj6_, adj7_),
        core_extension_(0b00100000));
    inv1_ = _mm256_mul_pd(inv1_, det_);

    /*
     * inv2_ = {adj11_, adj10_, adj9_, adj8_}
     */
    __m256d inv2_ = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(adj8_,  adj9_),
        _mm256_unpackhi_pd(adj10_, adj11_),
        core_extension_(0b00100000));
    inv2_ = _mm256_mul_pd(inv2_, det_);

    /*
     * inv3_ = {adj15_, adj14_, adj13_, adj12_}
     */
    __m256d inv3_ = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(adj12_, adj13_),
        _mm256_unpackhi_pd(adj14_, adj15_),
        core_extension_(0b00100000));
    inv3_ = _mm256_mul_pd(inv3_, det_);

    /*
     * Check for NaN and set the inverse to zero if the determinant is null.
     */
    __m256d mask_ = _mm256_or_pd(
        _mm256_cmplt_pd(det_, zero_),
        _mm256_cmpgt_pd(det_, zero_));
    inv0_ = _mm256_and_pd(inv0_, mask_);
    inv1_ = _mm256_and_pd(inv1_, mask_);
    inv2_ = _mm256_and_pd(inv2_, mask_);
    inv3_ = _mm256_and_pd(inv3_, mask_);

    mat4<double> result;
    simd_store(result, 0, inv0_);
    simd_store(result, 1, inv1_);
    simd_store(result, 2, inv2_);
    simd_store(result, 3, inv3_);
    return result;
}


/** ---------------------------------------------------------------------------
 * multiply
 * @brief Return the multiply of the matrix.
 */
template<>
core_inline_
vec2<double> multiply(const mat2<double> &a, const vec2<double> &v)
{
    /*
     * a0_ = {a0, a1}
     * a1_ = {a2, a3}
     * b_  = {b0, b1}
     */
    __m128d a0_ = simd_load(a, 0);
    __m128d a1_ = simd_load(a, 1);
    __m128d b_  = simd_load(v);
    /*
     * c0_ = {a0*b0 + a1*b1, a0*b0 + a1*b1}
     * c1_ = {a2*b0 + a3*b1, a2*b0 + a3*b1}
     *
     * _mm_unpackhi_pd(__m128d a, __m128d b)
     * dst[63:0]   := a[127:64]
     * dst[127:64] := b[127:64]
     */
    __m128d c0_ = simd128_dot_(a0_, b_);
    __m128d c1_ = simd128_dot_(a1_, b_);
    __m128d mul_ = _mm_unpackhi_pd(c0_, c1_);

    vec2<double> result;
    simd_store(result, mul_);
    return result;
}

template<>
core_inline_
mat2<double> multiply(const mat2<double> &a, const mat2<double> &b)
{
    /*
     * b0_ = {b0, b1}
     * b1_ = {b2, b3}
     */
    __m128d b0_ = simd_load(b, 0);
    __m128d b1_ = simd_load(b, 1);

    __m128d mul_[2];
    for (size_t i = 0; i < 2; ++i) {
        /*
         * a0_ = {a_n, a_n},    n = 0, 2
         * a1_ = {a_m, a_m},    m = 1, 3
         */
        __m128d a0_ = _mm_set1_pd(a(i,0));
        __m128d a1_ = _mm_set1_pd(a(i,1));
        /*
         * c0_ = {a_n * b0, a_n * b1}
         * c1_ = {a_m * b2, a_m * b3}
         */
        __m128d c0_ = _mm_mul_pd(a0_, b0_);
        __m128d c1_ = _mm_mul_pd(a1_, b1_);
        /*
         * mul_ = {a_n * b0 + a_m * b2,
         *         a_n * b1 + a_m * b3}
         */
        mul_[i] = _mm_add_pd(c0_, c1_);
    }

    mat2<double> result;
    simd_store(result, 0, mul_[0]);
    simd_store(result, 1, mul_[1]);
    return result;
}

template<>
core_inline_
vec3<double> multiply(const mat3<double> &a, const vec3<double> &v)
{
    /*
     * a0_ = {a0, a1, a2}
     * a1_ = {a3, a4, a5}
     * a2_ = {a6, a7, a8}
     * b_  = {b0, b1, b2}
     */
    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);
    __m256d b_  = simd_load(v);
    /*
     * c0_ = {a0*b0 + a1*b1 + a2*b2}
     * c1_ = {a3*b0 + a4*b1 + a2*b2}
     * c2_ = {a6*b0 + a7*b1 + a8*b2}
     *
     * _mm256_unpackhi_pd(__m256d a, __m256d b)
     * dst[63:0]    := a[127:64]
     * dst[127:64]  := b[127:64]
     * dst[191:128] := a[255:192]
     * dst[255:192] := b[255:192]
     */
    __m256d c0_   = simd256_dot_(a0_, b_);
    __m256d c1_   = simd256_dot_(a1_, b_);
    __m256d c2_   = simd256_dot_(a2_, b_);
    __m256d zero_ = _mm256_set1_pd(0.0);
    __m256d mul_  = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(c0_, c1_),
        _mm256_unpackhi_pd(c2_, zero_),
        core_extension_(0b00100000));

    vec3<double> result;
    simd_store(result, mul_);
    return result;
}

template<>
core_inline_
mat3<double> multiply(const mat3<double> &a, const mat3<double> &b)
{
    /*
     * b0_ = {b0, b1, b2}
     * b1_ = {b3, b4, b5}
     * b2_ = {b6, b7, b8}
     */
    __m256d b0_ = simd_load(b, 0);
    __m256d b1_ = simd_load(b, 1);
    __m256d b2_ = simd_load(b, 2);

    __m256d mul_[3];
    for (size_t i = 0; i < 3; ++i) {
        /*
         * a0_ = {a_n, a_n, a_n, a_n}   n = 0, 3, 6
         * a1_ = {a_m, a_m, a_m, a_m}   m = 1, 4, 7
         * a2_ = {a_l, a_l, a_l, a_l}   l = 2, 5, 8
         */
        __m256d a0_ = _mm256_set1_pd(a(i, 0));
        __m256d a1_ = _mm256_set1_pd(a(i, 1));
        __m256d a2_ = _mm256_set1_pd(a(i, 2));
        /*
         * c0_ = {a_n * b0, a_n * b1, a_n * b2}
         * c1_ = {a_m * b3, a_m * b4, a_m * b5}
         * c2_ = {a_l * b6, a_l * b7, a_l * b8}
         */
        __m256d c0_ = _mm256_mul_pd(a0_, b0_);
        __m256d c1_ = _mm256_mul_pd(a1_, b1_);
        __m256d c2_ = _mm256_mul_pd(a2_, b2_);
        /*
         * mul_ = {a_n * b0 + a_m * b3 + a_l * b6,
         *         a_n * b1 + a_m * b4 + a_l * b7,
         *         a_n * b2 + a_m * b5 + a_l * b8}
         */
         mul_[i] = _mm256_set1_pd(0.0);
         mul_[i] = _mm256_add_pd(mul_[i], c0_);
         mul_[i] = _mm256_add_pd(mul_[i], c1_);
         mul_[i] = _mm256_add_pd(mul_[i], c2_);
    }

    mat3<double> result;
    simd_store(result, 0, mul_[0]);
    simd_store(result, 1, mul_[1]);
    simd_store(result, 2, mul_[2]);
    return result;
}

template<>
core_inline_
vec4<double> multiply(const mat4<double> &a, const vec4<double> &v)
{
    /*
     * a0_ = {a0,  a1,  a2,  a3}
     * a1_ = {a4,  a5,  a6,  a7}
     * a2_ = {a8,  a9,  a10, a11}
     * a3_ = {a12, a13, a14, a15}
     */
    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);
    __m256d a3_ = simd_load(a, 3);
    __m256d b_  = simd_load(v);
    /*
     * c0_ = {a0*b0  + a1*b1  + a2*b2  + a3*b3}
     * c1_ = {a4*b0  + a5*b1  + a6*b2  + a7*b3}
     * c2_ = {a8*b0  + a9*b1  + a10*b2 + a11*b3}
     * c3_ = {a12*b0 + a13*b1 + a14*b2 + a15*b3}
     *
     * _mm256_unpackhi_pd(__m256d a, __m256d b)
     * dst[63:0]    := a[127:64]
     * dst[127:64]  := b[127:64]
     * dst[191:128] := a[255:192]
     * dst[255:192] := b[255:192]
     */
    __m256d c0_  = simd256_dot_(a0_, b_);
    __m256d c1_  = simd256_dot_(a1_, b_);
    __m256d c2_  = simd256_dot_(a2_, b_);
    __m256d c3_  = simd256_dot_(a3_, b_);
    __m256d mul_ = _mm256_permute2f128_pd(
        _mm256_unpackhi_pd(c0_, c1_),
        _mm256_unpackhi_pd(c2_, c3_),
        core_extension_(0b00100000));

    vec4<double> result;
    simd_store(result, mul_);
    return result;
}

template<>
core_inline_
mat4<double> multiply(const mat4<double> &a, const mat4<double> &b)
{
    /*
     * b0_ = {b0,  b1,  b2,  b3}
     * b1_ = {b4,  b5,  b6,  b7}
     * b2_ = {b8,  b9,  b10, b11}
     * b3_ = {b12, b13, b14, b15}
     */
    __m256d b0_ = simd_load(b, 0);
    __m256d b1_ = simd_load(b, 1);
    __m256d b2_ = simd_load(b, 2);
    __m256d b3_ = simd_load(b, 3);

    __m256d mul_[4];
    for (size_t i = 0; i < 4; ++i) {
        /*
         * a0_ = {a_n, a_n, a_n, a_n}   n = 0, 4, 8,  12
         * a1_ = {a_m, a_m, a_m, a_m}   m = 1, 5, 9,  13
         * a2_ = {a_l, a_l, a_l, a_l}   l = 2, 6, 10, 14
         * a3_ = {a_k, a_k, a_k, a_k}   k = 3, 7, 11, 15
         */
        __m256d a0_ = _mm256_set1_pd(a(i, 0));
        __m256d a1_ = _mm256_set1_pd(a(i, 1));
        __m256d a2_ = _mm256_set1_pd(a(i, 2));
        __m256d a3_ = _mm256_set1_pd(a(i, 3));
        /*
         * c0_ = {a_n * b0,  a_n * b1,  a_n * b2,  a_n * b3}
         * c1_ = {a_m * b4,  a_m * b5,  a_m * b6,  a_m * b7}
         * c2_ = {a_l * b8,  a_l * b9,  a_l * b10, a_l * b11}
         * c3_ = {a_k * b12, a_k * b13, a_k * b14, a_k * b15}
         */
        __m256d c0_ = _mm256_mul_pd(a0_, b0_);
        __m256d c1_ = _mm256_mul_pd(a1_, b1_);
        __m256d c2_ = _mm256_mul_pd(a2_, b2_);
        __m256d c3_ = _mm256_mul_pd(a3_, b3_);
        /*
         * mul_ = {a_n * b0 + a_m * b4 + a_l * b8  + a_k * b12,
         *         a_n * b1 + a_m * b5 + a_l * b9  + a_k * b13,
         *         a_n * b2 + a_m * b6 + a_l * b10 + a_k * b14,
         *         a_n * b3 + a_m * b7 + a_l * b11 + a_k * b15}
         */
         mul_[i] = _mm256_set1_pd(0.0);
         mul_[i] = _mm256_add_pd(mul_[i], c0_);
         mul_[i] = _mm256_add_pd(mul_[i], c1_);
         mul_[i] = _mm256_add_pd(mul_[i], c2_);
         mul_[i] = _mm256_add_pd(mul_[i], c3_);
    }

    mat4<double> result;
    simd_store(result, 0, mul_[0]);
    simd_store(result, 1, mul_[1]);
    simd_store(result, 2, mul_[2]);
    simd_store(result, 3, mul_[3]);
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

#endif /* CORE_GEOMETRY_ALGEBRA_SIMD_H_ */
