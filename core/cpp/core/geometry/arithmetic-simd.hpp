/*
 * arithmetic-simd.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_ARITHMETIC_SIMD_H_
#define CORE_GEOMETRY_ARITHMETIC_SIMD_H_

namespace core {
namespace math {

/** ---------------------------------------------------------------------------
 * abs
 * @brief Return the absolute value of the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/abs.xhtml
 *
 * |a| = max((0-a), a) = max(-a,a)
 */
template<>
core_inline_
vec2<double> abs(const vec2<double> &v)
{
    const __m128d zero = _mm_set1_pd(0.0);

    __m128d v_ = simd_load(v);
    __m128d abs_v_ = _mm_max_pd(_mm_sub_pd(zero, v_), v_);

    vec2<double> result;
    simd_store(result, abs_v_);
    return result;
}

template<>
core_inline_
vec3<double> abs(const vec3<double> &v)
{
    const __m256d zero = _mm256_set1_pd(0.0);

    __m256d v_ = simd_load(v);
    __m256d abs_v_ = _mm256_max_pd(_mm256_sub_pd(zero, v_), v_);

    vec3<double> result;
    simd_store(result, abs_v_);
    return result;
}

template<>
core_inline_
vec4<double> abs(const vec4<double> &v)
{
    const __m256d zero = _mm256_set1_pd(0.0);

    __m256d v_ = simd_load(v);
    __m256d abs_v_ = _mm256_max_pd(_mm256_sub_pd(zero, v_), v_);

    vec4<double> result;
    simd_store(result, abs_v_);
    return result;
}

template<>
core_inline_
mat2<double> abs(const mat2<double> &a)
{
    const __m128d zero = _mm_set1_pd(0.0);

    __m128d a0_ = simd_load(a, 0);
    __m128d a1_ = simd_load(a, 1);
    __m128d abs0_ = _mm_max_pd(_mm_sub_pd(zero, a0_), a0_);
    __m128d abs1_ = _mm_max_pd(_mm_sub_pd(zero, a1_), a1_);

    mat2<double> result;
    simd_store(result, 0, abs0_);
    simd_store(result, 1, abs1_);
    return result;
}

template<>
core_inline_
mat3<double> abs(const mat3<double> &a)
{
    const __m256d zero = _mm256_set1_pd(0.0);

    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);
    __m256d abs0_ = _mm256_max_pd(_mm256_sub_pd(zero, a0_), a0_);
    __m256d abs1_ = _mm256_max_pd(_mm256_sub_pd(zero, a1_), a1_);
    __m256d abs2_ = _mm256_max_pd(_mm256_sub_pd(zero, a2_), a2_);

    mat3<double> result;
    simd_store(result, 0, abs0_);
    simd_store(result, 1, abs1_);
    simd_store(result, 2, abs2_);
    return result;
}

template<>
core_inline_
mat4<double> abs(const mat4<double> &a)
{
    const __m256d zero = _mm256_set1_pd(0.0);

    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);
    __m256d a3_ = simd_load(a, 3);
    __m256d abs0_ = _mm256_max_pd(_mm256_sub_pd(zero, a0_), a0_);
    __m256d abs1_ = _mm256_max_pd(_mm256_sub_pd(zero, a1_), a1_);
    __m256d abs2_ = _mm256_max_pd(_mm256_sub_pd(zero, a2_), a2_);
    __m256d abs3_ = _mm256_max_pd(_mm256_sub_pd(zero, a3_), a3_);

    mat4<double> result;
    simd_store(result, 0, abs0_);
    simd_store(result, 1, abs1_);
    simd_store(result, 2, abs2_);
    simd_store(result, 3, abs3_);
    return result;
}


/** ---------------------------------------------------------------------------
 * sign
 * @brief Extract the sign of the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/sign.xhtml
 */
template<>
core_inline_
vec2<double> sign(const vec2<double> &v)
{
    const __m128d zero = _mm_set1_pd(0.0);
    const __m128d plus_one = _mm_set1_pd( 1.0);
    const __m128d minus_one = _mm_set1_pd(-1.0);

    __m128d a_ = simd_load(v);
    __m128d positive = _mm_and_pd(_mm_cmpgt_pd(a_, zero), plus_one);
    __m128d negative = _mm_and_pd(_mm_cmplt_pd(a_, zero), minus_one);
    __m128d sign_v_ = _mm_or_pd(positive, negative);

    vec2<double> result;
    simd_store(result, sign_v_);
    return result;
}

template<>
core_inline_
vec3<double> sign(const vec3<double> &v)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d plus_one = _mm256_set1_pd( 1.0);
    const __m256d minus_one = _mm256_set1_pd(-1.0);

    __m256d a_ = simd_load(v);
    __m256d positive = _mm256_and_pd(_mm256_cmpgt_pd(a_, zero), plus_one);
    __m256d negative = _mm256_and_pd(_mm256_cmplt_pd(a_, zero), minus_one);
    __m256d sign_v_ = _mm256_or_pd(positive, negative);

    vec3<double> result;
    simd_store(result, sign_v_);
    return result;
}

template<>
core_inline_
vec4<double> sign(const vec4<double> &v)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d plus_one = _mm256_set1_pd( 1.0);
    const __m256d minus_one = _mm256_set1_pd(-1.0);

    __m256d a_ = simd_load(v);
    __m256d positive = _mm256_and_pd(_mm256_cmpgt_pd(a_, zero), plus_one);
    __m256d negative = _mm256_and_pd(_mm256_cmplt_pd(a_, zero), minus_one);
    __m256d sign_v_ = _mm256_or_pd(positive, negative);

    vec4<double> result;
    simd_store(result, sign_v_);
    return result;
}

template<>
core_inline_
mat2<double> sign(const mat2<double> &a)
{
    const __m128d zero = _mm_set1_pd(0.0);
    const __m128d plus_one = _mm_set1_pd( 1.0);
    const __m128d minus_one = _mm_set1_pd(-1.0);

    __m128d a0_ = simd_load(a, 0);
    __m128d a1_ = simd_load(a, 1);

    __m128d sign0_ = _mm_or_pd(
        _mm_and_pd(_mm_cmpgt_pd(a0_, zero), plus_one),
        _mm_and_pd(_mm_cmplt_pd(a0_, zero), minus_one));
    __m128d sign1_ = _mm_or_pd(
        _mm_and_pd(_mm_cmpgt_pd(a1_, zero), plus_one),
        _mm_and_pd(_mm_cmplt_pd(a1_, zero), minus_one));

    mat2<double> result;
    simd_store(result, 0, sign0_);
    simd_store(result, 1, sign1_);
    return result;
}

template<>
core_inline_
mat3<double> sign(const mat3<double> &a)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d plus_one = _mm256_set1_pd( 1.0);
    const __m256d minus_one = _mm256_set1_pd(-1.0);

    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);

    __m256d sign0_ = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a0_, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a0_, zero), minus_one));
    __m256d sign1_ = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a1_, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a1_, zero), minus_one));
    __m256d sign2_ = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a2_, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a2_, zero), minus_one));

    mat3<double> result;
    simd_store(result, 0, sign0_);
    simd_store(result, 1, sign1_);
    simd_store(result, 2, sign2_);
    return result;
}

template<>
core_inline_
mat4<double> sign(const mat4<double> &a)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d plus_one = _mm256_set1_pd( 1.0);
    const __m256d minus_one = _mm256_set1_pd(-1.0);

    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);
    __m256d a3_ = simd_load(a, 3);

    __m256d sign0_ = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a0_, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a0_, zero), minus_one));
    __m256d sign1_ = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a1_, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a1_, zero), minus_one));
    __m256d sign2_ = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a2_, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a2_, zero), minus_one));
    __m256d sign3_ = _mm256_or_pd(
        _mm256_and_pd(_mm256_cmpgt_pd(a3_, zero), plus_one),
        _mm256_and_pd(_mm256_cmplt_pd(a3_, zero), minus_one));

    mat4<double> result;
    simd_store(result, 0, sign0_);
    simd_store(result, 1, sign1_);
    simd_store(result, 2, sign2_);
    simd_store(result, 3, sign3_);
    return result;
}


/** ---------------------------------------------------------------------------
 * floor
 * @brief Find the nearest integer less than or equal to the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/floor.xhtml
 */
template<>
core_inline_
vec2<double> floor(const vec2<double> &v)
{
    vec2<double> result;
    simd_store(result, _mm_floor_pd(simd_load(v)));
    return result;
}

template<>
core_inline_
vec3<double> floor(const vec3<double> &v)
{
    vec3<double> result;
    simd_store(result, _mm256_floor_pd(simd_load(v)));
    return result;
}

template<>
core_inline_
vec4<double> floor(const vec4<double> &v)
{
    vec4<double> result;
    simd_store(result, _mm256_floor_pd(simd_load(v)));
    return result;
}

template<>
core_inline_
mat2<double> floor(const mat2<double> &a)
{
    __m128d a0_ = simd_load(a, 0);
    __m128d a1_ = simd_load(a, 1);

    mat2<double> result;
    simd_store(result, 0, _mm_floor_pd(a0_));
    simd_store(result, 1, _mm_floor_pd(a1_));
    return result;
}

template<>
core_inline_
mat3<double> floor(const mat3<double> &a)
{
    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);

    mat3<double> result;
    simd_store(result, 0, _mm256_floor_pd(a0_));
    simd_store(result, 1, _mm256_floor_pd(a1_));
    simd_store(result, 2, _mm256_floor_pd(a2_));
    return result;
}

template<>
core_inline_
mat4<double> floor(const mat4<double> &a)
{
    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);
    __m256d a3_ = simd_load(a, 3);

    mat4<double> result;
    simd_store(result, 0, _mm256_floor_pd(a0_));
    simd_store(result, 1, _mm256_floor_pd(a1_));
    simd_store(result, 2, _mm256_floor_pd(a2_));
    simd_store(result, 3, _mm256_floor_pd(a3_));
    return result;
}


/** ---------------------------------------------------------------------------
 * round
 * @brief Find the nearest integer to the vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/round.xhtml
 *
 * @fn dst = __m128d _mm_round_pd(__m128d src, int rounding)
 * Round the packed double-precision (64-bit) floating-point elements
 * in src using the rounding parameter, and store the results as packed
 * double-precision floating-point elements in dst.
 *
 * Rounding is done according to the rounding[3:0] parameter:
 *
 *  round to nearest, and suppress exceptions
 *      (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC)
 *  round down, and suppress exceptions
 *      (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)
 *  round up, and suppress exceptions
 *      (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)
 *  truncate, and suppress exceptions
 *      (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)
 */
template<>
core_inline_
vec2<double> round(const vec2<double> &v)
{
    constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;
    vec2<double> result;
    simd_store(result, _mm_round_pd(simd_load(v), rounding));
    return result;
}

template<>
core_inline_
vec3<double> round(const vec3<double> &v)
{
    constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;
    vec3<double> result;
    simd_store(result, _mm256_round_pd(simd_load(v), rounding));
    return result;
}

template<>
core_inline_
vec4<double> round(const vec4<double> &v)
{
    constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;
    vec4<double> result;
    simd_store(result, _mm256_round_pd(simd_load(v), rounding));
    return result;
}

template<>
core_inline_
mat2<double> round(const mat2<double> &a)
{
    constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;

    __m128d a0_ = simd_load(a, 0);
    __m128d a1_ = simd_load(a, 1);

    mat2<double> result;
    simd_store(result, 0, _mm_round_pd(a0_, rounding));
    simd_store(result, 1, _mm_round_pd(a1_, rounding));
    return result;
}

template<>
core_inline_
mat3<double> round(const mat3<double> &a)
{
    constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;

    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);

    mat3<double> result;
    simd_store(result, 0, _mm256_round_pd(a0_, rounding));
    simd_store(result, 1, _mm256_round_pd(a1_, rounding));
    simd_store(result, 2, _mm256_round_pd(a2_, rounding));
    return result;
}

template<>
core_inline_
mat4<double> round(const mat4<double> &a)
{
    constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;

    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);
    __m256d a3_ = simd_load(a, 3);

    mat4<double> result;
    simd_store(result, 0, _mm256_round_pd(a0_, rounding));
    simd_store(result, 1, _mm256_round_pd(a1_, rounding));
    simd_store(result, 2, _mm256_round_pd(a2_, rounding));
    simd_store(result, 3, _mm256_round_pd(a3_, rounding));
    return result;
}


/** ---------------------------------------------------------------------------
 * ceil
 * @brief Find the nearest integer that is greater than or equal to the
 * vector elements.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/ceil.xhtml
 */
template<>
core_inline_
vec2<double> ceil(const vec2<double> &v)
{
    vec2<double> result;
    simd_store(result, _mm_ceil_pd(simd_load(v)));
    return result;
}

template<>
core_inline_
vec3<double> ceil(const vec3<double> &v)
{
    vec3<double> result;
    simd_store(result, _mm256_ceil_pd(simd_load(v)));
    return result;
}

template<>
core_inline_
vec4<double> ceil(const vec4<double> &v)
{
    vec4<double> result;
    simd_store(result, _mm256_ceil_pd(simd_load(v)));
    return result;
}

template<>
core_inline_
mat2<double> ceil(const mat2<double> &a)
{
    __m128d a0_ = simd_load(a, 0);
    __m128d a1_ = simd_load(a, 1);

    mat2<double> result;
    simd_store(result, 0, _mm_ceil_pd(a0_));
    simd_store(result, 1, _mm_ceil_pd(a1_));
    return result;
}

template<>
core_inline_
mat3<double> ceil(const mat3<double> &a)
{
    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);

    mat3<double> result;
    simd_store(result, 0, _mm256_ceil_pd(a0_));
    simd_store(result, 1, _mm256_ceil_pd(a1_));
    simd_store(result, 2, _mm256_ceil_pd(a2_));
    return result;
}

template<>
core_inline_
mat4<double> ceil(const mat4<double> &a)
{
    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);
    __m256d a3_ = simd_load(a, 3);

    mat4<double> result;
    simd_store(result, 0, _mm256_ceil_pd(a0_));
    simd_store(result, 1, _mm256_ceil_pd(a1_));
    simd_store(result, 2, _mm256_ceil_pd(a2_));
    simd_store(result, 3, _mm256_ceil_pd(a3_));
    return result;
}


/** ---------------------------------------------------------------------------
 * clamp
 * @brief Constrain the vector elements to lie inside the range
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/clamp.xhtml
 *
 * b = max(a, lo)
 * c = min(b, hi) = min(max(a, lo), hi);
 */
template<>
core_inline_
vec2<double> clamp(const vec2<double> &v, const double lo, const double hi)
{
    const __m128d lo_ = _mm_set1_pd(lo);
    const __m128d hi_ = _mm_set1_pd(hi);
    __m128d a_ = simd_load(v);

    vec2<double> result;
    simd_store(result, _mm_min_pd(_mm_max_pd(a_, lo_), hi_));
    return result;
}

template<>
core_inline_
vec3<double> clamp(const vec3<double> &v, const double lo, const double hi)
{
    const __m256d lo_ = _mm256_set1_pd(lo);
    const __m256d hi_ = _mm256_set1_pd(hi);
    __m256d a_ = simd_load(v);

    vec3<double> result;
    simd_store(result, _mm256_min_pd(_mm256_max_pd(a_, lo_), hi_));
    return result;
}

template<>
core_inline_
vec4<double> clamp(const vec4<double> &v, const double lo, const double hi)
{
    const __m256d lo_ = _mm256_set1_pd(lo);
    const __m256d hi_ = _mm256_set1_pd(hi);
    __m256d a_ = simd_load(v);

    vec4<double> result;
    simd_store(result, _mm256_min_pd(_mm256_max_pd(a_, lo_), hi_));
    return result;
}

template<>
core_inline_
mat2<double> clamp(const mat2<double> &a, const double lo, const double hi)
{
    const __m128d lo_ = _mm_set1_pd(lo);
    const __m128d hi_ = _mm_set1_pd(hi);

    __m128d a0_ = simd_load(a, 0);
    __m128d a1_ = simd_load(a, 1);

    mat2<double> result;
    simd_store(result, 0, _mm_min_pd(_mm_max_pd(a0_, lo_), hi_));
    simd_store(result, 1, _mm_min_pd(_mm_max_pd(a1_, lo_), hi_));
    return result;
}

template<>
core_inline_
mat3<double> clamp(const mat3<double> &a, const double lo, const double hi)
{
    const __m256d lo_ = _mm256_set1_pd(lo);
    const __m256d hi_ = _mm256_set1_pd(hi);

    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);

    mat3<double> result;
    simd_store(result, 0, _mm256_min_pd(_mm256_max_pd(a0_, lo_), hi_));
    simd_store(result, 1, _mm256_min_pd(_mm256_max_pd(a1_, lo_), hi_));
    simd_store(result, 2, _mm256_min_pd(_mm256_max_pd(a2_, lo_), hi_));
    return result;
}

template<>
core_inline_
mat4<double> clamp(const mat4<double> &a, const double lo, const double hi)
{
    const __m256d lo_ = _mm256_set1_pd(lo);
    const __m256d hi_ = _mm256_set1_pd(hi);

    __m256d a0_ = simd_load(a, 0);
    __m256d a1_ = simd_load(a, 1);
    __m256d a2_ = simd_load(a, 2);
    __m256d a3_ = simd_load(a, 3);

    mat4<double> result;
    simd_store(result, 0, _mm256_min_pd(_mm256_max_pd(a0_, lo_), hi_));
    simd_store(result, 1, _mm256_min_pd(_mm256_max_pd(a1_, lo_), hi_));
    simd_store(result, 2, _mm256_min_pd(_mm256_max_pd(a2_, lo_), hi_));
    simd_store(result, 3, _mm256_min_pd(_mm256_max_pd(a3_, lo_), hi_));
    return result;
}


/** ---------------------------------------------------------------------------
 * lerp
 * @brief Linearly interpolate between two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/mix.xhtml
 *
 * lerp = (1.0 - alpha) * lo + alpha * hi
 */
template<>
core_inline_
vec2<double> lerp(const vec2<double> lo, const vec2<double> hi, double alpha)
{
    const __m128d alpha_lo_ = _mm_set1_pd(1.0 - alpha);
    const __m128d alpha_hi_ = _mm_set1_pd(alpha);

    __m128d lo_ = simd_load(lo);
    __m128d hi_ = simd_load(hi);
    __m128d res_ = _mm_add_pd(
        _mm_mul_pd(alpha_lo_, lo_),
        _mm_mul_pd(alpha_hi_, hi_));

    vec2<double> result;
    simd_store(result, res_);
    return result;
}

template<>
core_inline_
vec3<double> lerp(const vec3<double> lo, const vec3<double> hi, double alpha)
{
    const __m256d alpha_lo_ = _mm256_set1_pd(1.0 - alpha);
    const __m256d alpha_hi_ = _mm256_set1_pd(alpha);

    __m256d lo_ = simd_load(lo);
    __m256d hi_ = simd_load(hi);
    __m256d res_ = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo_, lo_),
        _mm256_mul_pd(alpha_hi_, hi_));

    vec3<double> result;
    simd_store(result, res_);
    return result;
}

template<>
core_inline_
vec4<double> lerp(const vec4<double> lo, const vec4<double> hi, double alpha)
{
    const __m256d alpha_lo_ = _mm256_set1_pd(1.0 - alpha);
    const __m256d alpha_hi_ = _mm256_set1_pd(alpha);

    __m256d lo_ = simd_load(lo);
    __m256d hi_ = simd_load(hi);
    __m256d res_ = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo_, lo_),
        _mm256_mul_pd(alpha_hi_, hi_));

    vec4<double> result;
    simd_store(result, res_);
    return result;
}

template<>
core_inline_
mat2<double> lerp(const mat2<double> lo, const mat2<double> hi, double alpha)
{
    const __m128d alpha_lo_ = _mm_set1_pd(1.0 - alpha);
    const __m128d alpha_hi_ = _mm_set1_pd(alpha);

    __m128d res0_ = _mm_add_pd(
        _mm_mul_pd(alpha_lo_, simd_load(lo, 0)),
        _mm_mul_pd(alpha_hi_, simd_load(hi, 0)));
    __m128d res1_ = _mm_add_pd(
        _mm_mul_pd(alpha_lo_, simd_load(lo, 1)),
        _mm_mul_pd(alpha_hi_, simd_load(hi, 1)));

    mat2<double> result;
    simd_store(result, 0, res0_);
    simd_store(result, 1, res1_);
    return result;
}

template<>
core_inline_
mat3<double> lerp(const mat3<double> lo, const mat3<double> hi, double alpha)
{
    const __m256d alpha_lo_ = _mm256_set1_pd(1.0 - alpha);
    const __m256d alpha_hi_ = _mm256_set1_pd(alpha);

    __m256d res0_ = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo_, simd_load(lo, 0)),
        _mm256_mul_pd(alpha_hi_, simd_load(hi, 0)));
    __m256d res1_ = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo_, simd_load(lo, 1)),
        _mm256_mul_pd(alpha_hi_, simd_load(hi, 1)));
    __m256d res2_ = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo_, simd_load(lo, 2)),
        _mm256_mul_pd(alpha_hi_, simd_load(hi, 2)));

    mat3<double> result;
    simd_store(result, 0, res0_);
    simd_store(result, 1, res1_);
    simd_store(result, 2, res2_);
    return result;
}

template<>
core_inline_
mat4<double> lerp(const mat4<double> lo, const mat4<double> hi, double alpha)
{
    const __m256d alpha_lo_ = _mm256_set1_pd(1.0 - alpha);
    const __m256d alpha_hi_ = _mm256_set1_pd(alpha);

    __m256d res0_ = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo_, simd_load(lo, 0)),
        _mm256_mul_pd(alpha_hi_, simd_load(hi, 0)));
    __m256d res1_ = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo_, simd_load(lo, 1)),
        _mm256_mul_pd(alpha_hi_, simd_load(hi, 1)));
    __m256d res2_ = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo_, simd_load(lo, 2)),
        _mm256_mul_pd(alpha_hi_, simd_load(hi, 2)));
    __m256d res3_ = _mm256_add_pd(
        _mm256_mul_pd(alpha_lo_, simd_load(lo, 3)),
        _mm256_mul_pd(alpha_hi_, simd_load(hi, 3)));

    mat4<double> result;
    simd_store(result, 0, res0_);
    simd_store(result, 1, res1_);
    simd_store(result, 2, res2_);
    simd_store(result, 3, res3_);
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

#endif /* CORE_GEOMETRY_ARITHMETIC_SIMD_H_ */
