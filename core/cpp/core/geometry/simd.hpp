/*
 * simd.h
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_SIMD_H_
#define CORE_GEOMETRY_SIMD_H_

namespace core {
namespace math {

/** ---- Vector insert and extract intrinsics ---------------------------------
 *
 * @fn _mm256_set_m128d(__m128d hi, __m128d lo)
 * dst[127:0]   := lo[127:0]
 * dst[255:128] := hi[127:0]
 *
 * @fn _mm256_castpd128_pd256 (__m128d a)
 * Cast vector of type __m128d to type __m256d.
 * The upper 128 bits of the result are undefined.
 *
 * @fn _mm256_insertf128_pd(__m256d a, __m128d b, int mask)
 * dst[255:0] := a[255:0]
 * CASE mask[7:0] of
 *  0: dst[127:0]   := b[127:0]
 *  1: dst[255:128] := b[127:0]
 * ESAC
 */
#ifndef _mm256_set_m128d
#define _mm256_set_m128d(hi, lo) \
        _mm256_insertf128_pd(_mm256_castpd128_pd256(lo), (hi), 1)
#endif

/**
 * @fn _mm_cvtsd_f64 (__m128d a)
 * dst[63:0] := a[63:0]
 *
 * @fn _mm256_cvtsd_f64 (__m256d a)
 * dst[63:0] := a[63:0]
 *
 * @fn _mm256_extractf128_pd(__m256d a, const int mask)
 * CASE mask[7:0] of
 *  0: dst[127:0] := a[127:0]
 *  1: dst[127:0] := a[255:128]
 * ESAC
 */
#ifndef _mm256_cvtsd_f64
#define _mm256_cvtsd_f64(a) \
        _mm_cvtsd_f64(_mm256_extractf128_pd((a), 0))
#endif

/**
 * simd256_extract_
 *
 * @fn _mm_cvtsd_f64 (__m128d a)
 * dst[63:0] := a[63:0]
 *
 * @fn _mm256_extractf128_pd(__m256d a, const int mask)
 * dst[127:0] := (mask[7:0] == 0) ? a[127:0]
 * dst[127:0] := (mask[7:0] == 1) ? a[255:128]
 *
 * @fn _mm_permute_pd(__m128d a, int mask)
 * dst[63:0]   := (mask[0] == 0) ? a[63:0] : a[127:64]
 * dst[127:64] := (mask[1] == 0) ? a[63:0] : a[127:64]
 */
core_inline_
double simd256_extract_(__m256d a, int ix)
{
    /*
     * v_ = (ix == 0 || ix == 1) ? {a1, a0}
     * v_ = (ix == 3 || ix == 2) ? {a3, a2}
     */
    __m128d v_ = _mm256_extractf128_pd(a, (ix > 1));

    /*
     * v_ = (ix == 0) ? {a1, a0}
     * v_ = (ix == 1) ? {a0, a1}
     * v_ = (ix == 2) ? {a3, a2}
     * v_ = (ix == 3) ? {a2, a3}
     */
    v_ = _mm_permute_pd(
        v_,
       (ix%2 == 0) ? core_extension_(0b10) : core_extension_(0b01));

    return _mm_cvtsd_f64(v_);
}

/**
 * simd128_extract_
 */
core_inline_
double simd128_extract_(__m128d a, int ix)
{
    /*
     * v_ = (ix == 0) ? {a1, a0}
     * v_ = (ix == 1) ? {a0, a1}
     */
    __m128d v_ = _mm_permute_pd(
        a,
       (ix%2 == 0) ? core_extension_(0b10) : core_extension_(0b01));

    return _mm_cvtsd_f64(v_);
}


/** ---- Vector comparison intrinsics -----------------------------------------
 *
 * @fn _mm256_cmp_pd(__m256d a, __m256d b, const int imm8)
 *
 * dst[63:0]    := (a[63:0]    OP b[63:0])    ? 0xFFFFFFFFFFFFFFFF : 0
 * dst[127:64]  := (a[127:64]  OP b[127:64])  ? 0xFFFFFFFFFFFFFFFF : 0
 * dst[191:128] := (a[191:128] OP b[191:128]) ? 0xFFFFFFFFFFFFFFFF : 0
 * dst[255:192] := (a[255:192] OP b[255:192]) ? 0xFFFFFFFFFFFFFFFF : 0
 *
 * CASE(imm8[7:0]) OF
 *
 * 0:  OP := _CMP_EQ_OQ
 * 1:  OP := _CMP_LT_OS
 * 2:  OP := _CMP_LE_OS
 * 3:  OP := _CMP_UNORD_Q
 * 4:  OP := _CMP_NEQ_UQ
 * 5:  OP := _CMP_NLT_US
 * 6:  OP := _CMP_NLE_US
 * 7:  OP := _CMP_ORD_Q
 * 8:  OP := _CMP_EQ_UQ
 * 9:  OP := _CMP_NGE_US
 * 10: OP := _CMP_NGT_US
 * 11: OP := _CMP_FALSE_OQ
 * 12: OP := _CMP_NEQ_OQ
 * 13: OP := _CMP_GE_OS
 * 14: OP := _CMP_GT_OS
 * 15: OP := _CMP_TRUE_UQ
 * 16: OP := _CMP_EQ_OS
 * 17: OP := _CMP_LT_OQ
 * 18: OP := _CMP_LE_OQ
 * 19: OP := _CMP_UNORD_S
 * 20: OP := _CMP_NEQ_US
 * 21: OP := _CMP_NLT_UQ
 * 22: OP := _CMP_NLE_UQ
 * 23: OP := _CMP_ORD_S
 * 24: OP := _CMP_EQ_US
 * 25: OP := _CMP_NGE_UQ
 * 26: OP := _CMP_NGT_UQ
 * 27: OP := _CMP_FALSE_OS
 * 28: OP := _CMP_NEQ_OS
 * 29: OP := _CMP_GE_OQ
 * 30: OP := _CMP_GT_OQ
 * 31: OP := _CMP_TRUE_US
 */
#ifndef _mm256_cmplt_pd
#define _mm256_cmplt_pd(a, b) \
        _mm256_cmp_pd((a), (b), 1)
#endif

#ifndef _mm256_cmple_pd
#define _mm256_cmple_pd(a, b) \
        _mm256_cmp_pd((a), (b), 2)
#endif

#ifndef _mm256_cmpge_pd
#define _mm256_cmpge_pd(a, b) \
        _mm256_cmp_pd((a), (b), 13)
#endif

#ifndef _mm256_cmpgt_pd
#define _mm256_cmpgt_pd(a, b) \
        _mm256_cmp_pd((a), (b), 14)
#endif

#ifndef _mm256_cmpeq_pd
#define _mm256_cmpeq_pd(a, b) \
        _mm256_cmp_pd((a), (b), 16)
#endif

#ifndef _mm256_cmpne_pd
#define _mm256_cmpne_pd(a, b) \
        _mm256_cmp_pd((a), (b), 28)
#endif


/** ---- Vector bitwise intrinsics --------------------------------------------
 *
 * @fn __m128d _mm_and_pd(__m128d a, __m128d b)
 * @brief Compute the bitwise AND of packed double-precision (64-bit)
 * floating-point elements in a and b, and store the results in dst.
 *
 * FOR j := 0 to 1
 * i := j*64
 * dst[i+63:i] := (a[i+63:i] AND b[i+63:i])
 * ENDFOR
 *
 *
 * @fn __m128d _mm_andnot_pd(__m128d a, __m128d b)
 * @brief Compute the bitwise NOT of packed double-precision (64-bit)
 * floating-point elements in a and then AND with b, and store the results
 * in dst.
 *
 * FOR j := 0 to 1
 *  i := j*64
 *  dst[i+63:i] := ((NOT a[i+63:i]) AND b[i+63:i])
 * ENDFOR
 *
 *
 * @fn __m128d _mm_or_pd(__m128d a, __m128d b)
 * @brief Compute the bitwise OR of packed double-precision (64-bit)
 * floating-point elements in a and b, and store the results in dst.
 *
 * FOR j := 0 to 1
 *  i := j*64
 *  dst[i+63:i] := a[i+63:i] OR b[i+63:i]
 * ENDFOR
 *
 *
 * @fn __m256d _mm256_and_pd(__m256d a, __m256d b)
 * @brief Compute the bitwise AND of packed double-precision (64-bit)
 * floating-point elements in a and b, and store the results in dst.
 *
 * FOR j := 0 to 3
 *  i := j*64
 *  dst[i+63:i] := (a[i+63:i] AND b[i+63:i])
 * ENDFOR
 * dst[MAX:256] := 0
 *
 *
 * @fn __m256d _mm256_andnot_pd(__m256d a, __m256d b)
 * @brief Compute the bitwise NOT of packed double-precision (64-bit)
 * floating-point elements in a and then AND with b, and store the results
 * in dst.
 *
 * FOR j := 0 to 3
 *  i := j*64
 *  dst[i+63:i] := ((NOT a[i+63:i]) AND b[i+63:i])
 * ENDFOR
 * dst[MAX:256] := 0
 *
 *
 * @fn __m256d _mm256_or_pd(__m256d a, __m256d b)
 * @brief Compute the bitwise OR of packed double-precision (64-bit)
 * floating-point elements in a and b, and store the results in dst.
 *
 * FOR j := 0 to 3
 *  i := j*64
 *  dst[i+63:i] := a[i+63:i] OR b[i+63:i]
 * ENDFOR
 * dst[MAX:256] := 0
 */


/** ---- Vector shuffle intrinsics --------------------------------------------
 *
 * simd128_shuffle_
 *
 * @brief Shuffle double-precision (64-bit) floating-point elements
 * using the control in mask, and store the result in dst.
 *
 * @fn _mm_shuffle_pd(__m128d a, __m128d b, int mask)
 * dst[63:0]   := (mask[0] == 0) ? a[63:0] : a[127:64]
 * dst[127:64] := (mask[1] == 0) ? b[63:0] : b[127:64]
 */
core_inline_
__m128d simd128_shuffle_(__m128d a, __m128d b, const int mask)
{
    return _mm_shuffle_pd(a, b, mask);
}

/**
 * simd256_shuffle_
 *
 * @brief Shuffle double-precision (64-bit) floating-point elements within
 * 128-bit lanes using the control in mask, and store the result in dst.
 *
 * @fn _mm256_shuffle_pd(__m256d a, __m256d b, const int mask)
 * dst[63:0]    := (mask[0] == 0) ? a[63:0]    : a[127:64]
 * dst[127:64]  := (mask[1] == 0) ? b[63:0]    : b[127:64]
 * dst[191:128] := (mask[2] == 0) ? a[191:128] : a[255:192]
 * dst[255:192] := (mask[3] == 0) ? b[191:128] : b[255:192]
 */
core_inline_
__m256d simd256_shuffle_(__m256d a, __m256d b, const int mask)
{
    return _mm256_shuffle_pd(a, b, mask);
}

/**
 * simd256_shuffle2f_
 *
 * @brief Shuffle 128-bits(composed of 2 packed double-precision (64-bit)
 * floating-point elements) selected by mask from a and b, and store
 * the result in dst.
 *
 * @fn _mm256_set_m128d(__m128d hi, __m128d lo)
 * dst[127:0]   := lo[127:0]
 * dst[255:128] := hi[127:0]
 *
 * @fn _mm256_extractf128_pd(__m256d a, const int mask)
 * dst[127:0] := (mask[7:0] == 0) ? a[127:0]
 * dst[127:0] := (mask[7:0] == 1) ? a[255:128]
 *
 * Operation:
 *
 * dst[127:0]   := (masklo[0] == 0) ? a[127:0] : a[255:128]
 * dst[255:128] := (maskhi[0] == 0) ? b[127:0] : b[255:128]
 */
core_inline_
__m256d simd256_shuffle2f_(
    __m256d a,
    const int masklo,
    __m256d b,
    const int maskhi)
{
    return _mm256_set_m128d(_mm256_extractf128_pd(b, maskhi),
                            _mm256_extractf128_pd(a, masklo));
}


/** ---- Vector permute intrinsics --------------------------------------------
 *
 * simd128_permute_
 *
 * @brief Shuffle double-precision (64-bit) floating-point elements
 * in a using the control in mask, and store the result in dst.
 *
 * @fn _mm_permute_pd(__m128d a, int mask)
 * dst[63:0]    := (mask[0] == 0) ? a[63:0] : a[127:64]
 * dst[127:64]  := (mask[1] == 0) ? a[63:0] : a[127:64]
 */
core_inline_
__m128d simd128_permute_(__m128d a, int mask)
{
    return _mm_permute_pd(a, mask);
}


/**
 * simd256_permute_
 *
 * @brief Shuffle double-precision (64-bit) floating-point elements within
 * 128-bit lanes using the control in mask, and store the result in dst.
 *
 * @fn _mm256_permute_pd(__m256d a, int mask)
 * dst[63:0]    := (mask[0] == 0) ? a[63:0]    : a[127:64]
 * dst[127:64]  := (mask[1] == 0) ? a[63:0]    : a[127:64]
 * dst[191:128] := (mask[2] == 0) ? a[191:128] : a[255:192]
 * dst[255:192] := (mask[3] == 0) ? a[191:128] : a[255:192]
 */
core_inline_
__m256d simd256_permute_(__m256d a, int mask)
{
    return _mm256_permute_pd(a, mask);
}


/**
 * simd256_permute2f_
 *
 * @brief Shuffle 128-bits(composed of 2 packed double-precision (64-bit)
 * floating-point elements) selected by mask from a and b, and store
 * the result in dst.
 *
 * @fn _mm256_permute2f128_pd(__m256d a, __m256d b, int mask)
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
core_inline_
__m256d simd256_permute2f_(__m256d a, __m256d b, int mask)
{
    return _mm256_permute2f128_pd(a, b, mask);
}


/** ---- Vector reverse intrinsics --------------------------------------------
 *
 * simd128_reverse_
 *
 * @brief Reverse the order of two double-precision (64-bit) floating-point
 * elements.
 *
 * @fn _mm_permute_pd(__m128d a, int mask)
 * dst[63:0]    := (mask[0] == 0) ? a[63:0] : a[127:64]
 * dst[127:64]  := (mask[1] == 0) ? a[63:0] : a[127:64]
 */
core_inline_
__m128d simd128_reverse_(__m128d a)
{
    /*
     * {a1, a0} -> {a0, a1}
     */
    return _mm_permute_pd(a, core_extension_(0b01));
}

/**
 * simd256_reverse_
 *
 * @brief Reverse the order of two double-precision (64-bit) floating-point
 * elements within 128-bit lanes.
 *
 * @fn _mm256_permute_pd(__m256d a, int mask)
 * dst[63:0]    := dst[127:64] (mask[0] = 1)
 * dst[127:64]  := dst[63:0]   (mask[1] = 0)
 * dst[191:128] := dst[255:192] (mask[2] = 1)
 * dst[255:192] := dst[191:128] (mask[3] = 0)
 *
 * @fn _mm256_permute2f128_pd(__m256d a, __m256d b, int mask)
 * dst[127:0]   := a[255:128]  (mask[1:0] = 1, mask[3] = 0)
 * dst[255:128] := a[127:0]    (mask[5:4] = 0, mask[7] = 0)
 */
core_inline_
__m256d simd256_reverse_(__m256d a)
{
    /*
     * {a3, a2, a1, a0} -> {a2, a3, a0, a1}
     */
    __m256d v_ = _mm256_permute_pd(a, core_extension_(0b0101));      /* 0x5 */
    /*
     * {a3, a2, a1, a0} -> {a1, a0, a3, a2}
     */
    return _mm256_permute2f128_pd(v_, v_, core_extension_(0b0001));  /* 0x1 */
}


/** ---- Vector arithmetic intrinsics -----------------------------------------
 *
 * @fn _mm_rsqrt_pd
 *
 * dst[63:0]    := 1.0 / (SQRT(a[63:0])
 * dst[127:64]  := 1.0 / (SQRT(a[127:64])
 */
core_inline_
__m128d _mm_rsqrt_pd(__m128d a)
{
    return _mm_div_pd(_mm_set1_pd(1.0), _mm_sqrt_pd(a));
}

/**
 * @fn _mm256_rsqrt_pd
 *
 * dst[63:0]    := 1.0 / (SQRT(a[63:0])
 * dst[127:64]  := 1.0 / (SQRT(a[127:64])
 * dst[191:128] := 1.0 / (SQRT(a[191:128])
 * dst[255:192] := 1.0 / (SQRT(a[255:192])
 */
core_inline_
__m256d _mm256_rsqrt_pd(__m256d a)
{
    return _mm256_div_pd(_mm256_set1_pd(1.0), _mm256_sqrt_pd(a));
}


/**
 * simd128_rsqrt_
 *
 * @brief Inverse square root of two double-precision (64-bit) elements.
 *
 * @fn __m128 _mm_cvtpd_ps(__m128d a)
 * dst[31:0]   := Convert_FP64_To_FP32(a[63:0])
 * dst[63:32]  := Convert_FP64_To_FP32(a[127:64])
 *
 * @fn __m128 _mm_rsqrt_ps(__m128 a)
 * dst[31:0]   :=  APPROXIMATE(1.0 / SQRT(a[31:0]))
 * dst[63:32]  :=  APPROXIMATE(1.0 / SQRT(a[63:32]))
 * dst[95:64]  :=  APPROXIMATE(1.0 / SQRT(a[95:64]))
 * dst[127:96] :=  APPROXIMATE(1.0 / SQRT(a[127:96]))
 *
 * @fn __m256d _mm256_cvtps_pd(__m128 a)
 * dst[63:0]   := Convert_FP32_To_FP64(a[31:0])
 * dst[127:64] := Convert_FP32_To_FP64(a[63:32])
 */
core_inline_
__m128d simd128_rsqrt_(__m128d x)
{
    const __m128d half_ = _mm_set1_pd(0.5);
    const __m128d one_half_ = _mm_set1_pd(1.5);
    /*
     * Convert packed double-precision (64-bit) elements
     * to packed single-precision (32-bit) elements.
     *
     * Compute approximate reciprocal square root of
     * packed single-precision (32-bit) elements.
     *
     * Convert packed single-precision (32-bit) elements
     * to packed double-precision (64-bit) elements.
     */
    __m128d y1_ = _mm_cvtps_pd(
        _mm_rsqrt_ps(
            _mm_cvtpd_ps(x)));
    /*
     * Newton-Raphson optimization of the inverse square root estimate.
     *
     * y(k+1) = y(k) - f(k) / f'(k)
     *        = (3/2)*y(k) - (x/2)*y(k)*y(k)*y(k)
     *        = y(k)*(1.5 - 0.5*x*y(k)*y(k))
     */
    __m128d x2_ = _mm_mul_pd(half_, x);

    __m128d y2_ = _mm_mul_pd(
        y1_, _mm_sub_pd(
            one_half_, _mm_mul_pd(
                x2_, _mm_mul_pd(y1_, y1_))));

    __m128d y3_ = _mm_mul_pd(
        y2_, _mm_sub_pd(
            one_half_, _mm_mul_pd(
                x2_, _mm_mul_pd(y2_, y2_))));

    __m128d y4_ = _mm_mul_pd(
        y3_, _mm_sub_pd(
            one_half_, _mm_mul_pd(
                x2_, _mm_mul_pd(y3_, y3_))));

    return y4_;
}

/**
 * simd256_rsqrt_
 *
 * @brief Inverse square root of four double-precision (64-bit) elements.
 *
 * @fn __m128d _mm256_cvtpd_ps(__m256d a)
 * dst[31:0]    := Convert_FP64_To_FP32(a[63:0])
 * dst[63:32]   := Convert_FP64_To_FP32(a[127:64])
 * dst[95:64]   := Convert_FP64_To_FP32(a[191:128])
 * dst[127:96]  := Convert_FP64_To_FP32(a[255:192])
 *
 * @fn __m128 _mm_rsqrt_ps(__m128 a)
 * dst[31:0]   :=  APPROXIMATE(1.0 / SQRT(a[31:0]))
 * dst[63:32]  :=  APPROXIMATE(1.0 / SQRT(a[63:32]))
 * dst[95:64]  :=  APPROXIMATE(1.0 / SQRT(a[95:64]))
 * dst[127:96] :=  APPROXIMATE(1.0 / SQRT(a[127:96]))
 *
 * @fn __m256d _mm256_cvtps_pd(__m128 a)
 * dst[63:0]    := Convert_FP32_To_FP64(a[31:0])
 * dst[127:64]  := Convert_FP32_To_FP64(a[63:32])
 * dst[191:128] := Convert_FP32_To_FP64(a[95:64])
 * dst[255:192] := Convert_FP32_To_FP64(a[127:96])
 */
core_inline_
__m256d simd256_rsqrt_(__m256d x)
{
    const __m256d half_ = _mm256_set1_pd(0.5);
    const __m256d one_half_ = _mm256_set1_pd(1.5);

    /*
     * Convert packed double-precision (64-bit) elements
     * to packed single-precision (32-bit) elements.
     *
     * Compute approximate reciprocal square root of
     * packed single-precision (32-bit) elements.
     *
     * Convert packed single-precision (32-bit) elements
     * to packed double-precision (64-bit) elements.
     */
    __m256d y1_ = _mm256_cvtps_pd(_mm_rsqrt_ps(_mm256_cvtpd_ps(x)));

    /*
     * Newton-Raphson optimization of the inverse square root estimate.
     *
     * y(k+1) = y(k) - f(k) / f'(k)
     *        = (3/2)*y(k) - (x/2)*y(k)*y(k)*y(k)
     *        = y(k)*(1.5 - 0.5*x*y(k)*y(k))
     */
    __m256d x2_ = _mm256_mul_pd(half_, x);

    __m256d y2_ = _mm256_mul_pd(
        y1_, _mm256_sub_pd(
            one_half_, _mm256_mul_pd(
                x2_, _mm256_mul_pd(y1_, y1_))));

    __m256d y3_ = _mm256_mul_pd(
        y2_, _mm256_sub_pd(
            one_half_, _mm256_mul_pd(
                x2_, _mm256_mul_pd(y2_, y2_))));

    __m256d y4_ = _mm256_mul_pd(
        y3_, _mm256_sub_pd(
            one_half_, _mm256_mul_pd(
                x2_, _mm256_mul_pd(y3_, y3_))));

    return y4_;
}

/**
 * simd128_dot_
 *
 * @brief Dot product of two double precision (64-bit) elements.
 *
 * @fn _mm_mul_pd(__m128d a, __m128d b)
 * dst[63:0]   := a[63:0]   * a[63:0]
 * dst[127:64] := b[127:64] * b[127:64]
 *
 * @fn _mm_hadd_pd(__m128d a, __m128d b)
 * dst[63:0]   := a[127:64] + a[63:0]
 * dst[127:64] := b[127:64] + b[63:0]
 */
core_inline_
__m128d simd128_dot_(__m128d a, __m128d b)
{
    /*
     * {a1*b1, a0*b0}
     */
    __m128d ymul_ = _mm_mul_pd(a, b);
    /*
     * {a1*b1 + a0*b0, a1*b1 + a0*b0}
     */
    __m128d ydot_ = _mm_hadd_pd(ymul_, ymul_);

    return ydot_;
}

/**
 * simd256_dot_
 *
 * @brief Dot product of four double precision (64-bit) elements.
 *
 * @fn _mm256_mul_pd(__m256d a, __m256d b)
 * dst[63:0]    := a[63:0]    * a[63:0]
 * dst[127:64]  := b[127:64]  * b[127:64]
 * dst[191:128] := b[191:128] * b[191:128]
 * dst[255:192] := b[255:192] * b[255:192]
 *
 * @fn _mm256_hadd_pd(__m256d a, __m256d b)
 * dst[63:0]    := a[127:64]  + a[63:0]
 * dst[127:64]  := b[127:64]  + b[63:0]
 * dst[191:128] := a[255:192] + a[191:128]
 * dst[255:192] := b[255:192] + b[191:128]
 *
 * @fn _mm256_add_pd(__m128d a, __m128d b)
 * dst[63:0]    := a[63:0]    + a[63:0]
 * dst[127:64]  := b[127:64]  + b[127:64]
 * dst[191:128] := b[191:128] + b[191:128]
 * dst[255:192] := b[255:192] + b[255:192]
 */
core_inline_
__m256d simd256_dot_(__m256d a, __m256d b)
{
    /*
     * {a3*b3, a2*b2, a1*b1, a0*b0}
     */
    __m256d ymul_ = _mm256_mul_pd(a, b);
    /*
     * {a3*b3 + a2*b2,
     *  a3*b3 + a2*b2,
     *  a1*b1 + a0*b0,
     *  a1*b1 + a0*b0}
     */
    __m256d yadd0_ = _mm256_hadd_pd(ymul_, ymul_);
    /*
     * {a3, a2, a1, a0} -> {a1, a0, a3, a2}
     */
    __m256d yadd1_ = _mm256_permute2f128_pd(
        yadd0_,
        yadd0_,
        core_extension_(0b0001)  /*0x1*/);
    /*
     * {a3*b3 + a2*b2 + a1*b1 + a0*b0,
     *  a3*b3 + a2*b2 + a1*b1 + a0*b0,
     *  a3*b3 + a2*b2 + a1*b1 + a0*b0,
     *  a3*b3 + a2*b2 + a1*b1 + a0*b0}
     */
    __m256d ydot_ = _mm256_add_pd(yadd0_, yadd1_);

    return ydot_;
}

/**
 * simd128_norm_
 *
 * @brief Euclidean norm of two double-precision (64-bit) elements.
 */
core_inline_
__m128d simd128_norm_(__m128d a)
{
    /*
     * sqrt{a1*a1 + a0*a0,
     *      a1*a1 + a0*a0}
     */
    return _mm_sqrt_pd(simd128_dot_(a, a));
}

/**
 * simd256_norm_
 *
 * @brief Euclidean norm of four double-precision (64-bit) elements.
 */
core_inline_
__m256d simd256_norm_(__m256d a)
{
    /*
     * sqrt{a3*a3 + a2*a2 + a1*a1 + a0*a0,
     *      a3*a3 + a2*a2 + a1*a1 + a0*a0,
     *      a3*a3 + a2*a2 + a1*a1 + a0*a0,
     *      a3*a3 + a2*a2 + a1*a1 + a0*a0}
     */
     return _mm256_sqrt_pd(simd256_dot_(a, a));
}

/**
 * simd128_normalize_
 *
 * @brief Normalize two double precision (64-bit) elements by their
 * Euclidean norm.
 */
core_inline_
__m128d simd128_normalize_(__m128d a)
{
    /*
     * a / sqrt{a1*b1 + a0*b0,
     *          a1*b1 + a0*b0}
     */
    __m128d ydot_ = simd128_dot_(a, a);
    __m128d ynorm_ = simd128_rsqrt_(ydot_);  /* _mm_rsqrt_pd(ydot_) */

    return _mm_mul_pd(a, ynorm_);
}

/**
 * simd256_normalize_
 *
 * @brief Normalize four double precision (64-bit) elements by their
 * Euclidean norm.
 */
core_inline_
__m256d simd256_normalize_(__m256d a)
{
    /*
     * a / sqrt{a3*b3 + a2*b2 + a1*b1 + a0*b0,
     *          a3*b3 + a2*b2 + a1*b1 + a0*b0,
     *          a3*b3 + a2*b2 + a1*b1 + a0*b0,
     *          a3*b3 + a2*b2 + a1*b1 + a0*b0}
     */
    __m256d ydot_ = simd256_dot_(a, a);
    __m256d ynorm_ = simd256_rsqrt_(ydot_);  /* _mm256_rsqrt_pd(ydot_) */

     return _mm256_mul_pd(a, ynorm_);
}


/** ---- Matrix transpose intrinsics ------------------------------------------
 *
 * simd128_transpose_
 *
 * @brief Return the transpose a 2x2 matrix represented as 2 vector-rows
 * of dimension 2.
 *
 * @fn _mm_shuffle_pd(__m128d a, __m128d b, int mask)
 * dst[63:0]   := (mask[0] == 0) ? a[63:0] : a[127:64]
 * dst[127:64] := (mask[1] == 0) ? b[63:0] : b[127:64]
 *
 * @note Operation:
 * row0 = {a1, a0} -> {a2, a0}
 * row1 = {a3, a2} -> {a3, a1}
 */
core_inline_
void simd128_transpose_(__m128d(&row)[2])
{
    /*
     * r0_ = {a1, a0}
     * r1_ = {a3, a2}
     */
    __m128d r0_ = row[0];
    __m128d r1_ = row[1];
    /*
     * t0_ = {a2, a0}
     * t1_ = {a3, a1}
     */
    __m128d t0_ = _mm_shuffle_pd(r0_, r1_, core_extension_(0b00));
    __m128d t1_ = _mm_shuffle_pd(r0_, r1_, core_extension_(0b11));

    row[0] = t0_;
    row[1] = t1_;
}

/**
 * simd256_transpose_
 *
 * @brief Return the transpose of a 4x4 matrix represented as 4 vector-rows
 * of dimension 4.
 *
 * @fn _mm256_permute2f128_pd(__m256d a, __m256d b, int mask)
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
 *
 * @fn _mm256_unpackhi_pd(__m256d a, __m256d b)
 * dst[63:0]    := a[127:64]
 * dst[127:64]  := b[127:64]
 * dst[191:128] := a[255:192]
 * dst[255:192] := b[255:192]
 *
 * @fn _mm256_unpacklo_pd(__m256d a, __m256d b)
 * dst[63:0]    := a[63:0]
 * dst[127:64]  := b[63:0]
 * dst[191:128] := a[191:128]
 * dst[255:192] := b[191:128]
 *
 * @note Operation:
 * row0 = { a3,  a2,  a1,  a0} -> {a12, a8,  a4, a0}
 * row1 = { a7,  a6,  a5,  a4} -> {a13, a9,  a5, a1}
 * row2 = {a11, a10,  a9,  a8} -> {a14, a10, a6, a2}
 * row3 = {a15, a14, a13, a12} -> {a15, a11, a7, a3}
 */
core_inline_
void simd256_transpose_(__m256d (&row)[4])
{
    /*
     * r0_ = { a3,  a2,  a1,  a0}
     * r1_ = { a7,  a6,  a5,  a4}
     * r2_ = {a11, a10,  a9,  a8}
     * r3_ = {a15, a14, a13, a12}
     */
    __m256d r0_ = row[0];
    __m256d r1_ = row[1];
    __m256d r2_ = row[2];
    __m256d r3_ = row[3];
    /*
     * r4_ = { a9,  a8, a11, a10}  (r2_, r2_) (mask[5:4] = 0, mask[1:0] = 1)
     * r5_ = {a13, a12, a15, a14}  (r3_, r3_) (mask[5:4] = 0, mask[1:0] = 1)
     */
    __m256d r4_ = _mm256_permute2f128_pd(
        r2_, r2_, core_extension_(0b00000001));

    __m256d r5_ = _mm256_permute2f128_pd(
        r3_, r3_, core_extension_(0b00000001));
    /*
     * p1_ = { a6, a2,  a4,  a0}
     * p2_ = { a7, a3,  a5,  a1}
     * p3_ = {a12, a8, a14, a10}
     * p4_ = {a13, a9, a15, a11}
     */
    __m256d p1_ = _mm256_unpacklo_pd(r0_, r1_);
    __m256d p2_ = _mm256_unpackhi_pd(r0_, r1_);
    __m256d p3_ = _mm256_unpacklo_pd(r4_, r5_);
    __m256d p4_ = _mm256_unpackhi_pd(r4_, r5_);
    /*
     * t0_ = {a12, a8,  a4,  a0}   (p1_, p3_) (mask[5:4] = 3, mask[1:0] = 0)
     * t1_ = {a13, a9,  a5,  a1}   (p4_, p2_) (mask[5:4] = 3, mask[1:0] = 0)
     * t2_ = { a6, a2, a14, a10}   (p1_, p3_) (mask[5:4] = 1, mask[1:0] = 2)
     * t3_ = { a7, a3, a15, a11}   (p4_, p2_) (mask[5:4] = 1, mask[1:0] = 2)
     */
    __m256d t0_ = _mm256_permute2f128_pd(
        p1_, p3_, core_extension_(0b00110000));

    __m256d t1_ = _mm256_permute2f128_pd(
        p2_, p4_, core_extension_(0b00110000));

    __m256d t2_ = _mm256_permute2f128_pd(
        p1_, p3_, core_extension_(0b00010010));

    __m256d t3_ = _mm256_permute2f128_pd(
        p2_, p4_, core_extension_(0b00010010));
    /*
     * t4_ = {a14, a10, a6,  a2}   (t2_, t2_) (mask[5:4] = 0, mask[1:0] = 1)
     * t5_ = {a15, a11, a7,  a3}   (t3_, t3_) (mask[5:4] = 0, mask[1:0] = 1)
     */
    __m256d t4_ = _mm256_permute2f128_pd(
        t2_, t2_, core_extension_(0b00000001));

    __m256d t5_ = _mm256_permute2f128_pd(
        t3_, t3_, core_extension_(0b00000001));

    row[0] = t0_;
    row[1] = t1_;
    row[2] = t4_;
    row[3] = t5_;
}


/** ---- Matrix determinant intrinsics ----------------------------------------
 *
 * simd256_det_
 *
 * @brief Return the determinant of a 2x2 matrix represented as 4
 * packed double-precision (64-bit) floating-point elements.
 *
 * @fn _mm256_permute_pd(__m256d a, int mask)
 * dst[63:0]    := (mask[0] == 0) ? a[63:0]    : a[127:64]
 * dst[127:64]  := (mask[1] == 0) ? a[63:0]    : a[127:64]
 * dst[191:128] := (mask[2] == 0) ? a[191:128] : a[255:192]
 * dst[255:192] := (mask[3] == 0) ? a[191:128] : a[255:192]
 *
 * @fn _mm256_hsub_pd(__m256d a, __m256d b)
 * dst[63:0]    := a[63:0]    - a[127:64]
 * dst[127:64]  := b[63:0]    - b[127:64]
 * dst[191:128] := a[191:128] - a[255:192]
 * dst[255:192] := b[191:128] - b[255:192]
 */
core_inline_
__m256d simd256_det_(__m256d a)
{
    /*
     * r0_ = {a3, a2, a1, a0}
     * r1_ = {a0, a1, a2, a3}
     * r2_ = {a3*a0, a2*a1, a1*a2, a0*a3}
     * r3_ = {a2*a1, a3*a0, a1*a2, a0*a3}
     *
     * det_ = husb(r3_, r3_)
     *      = {a3*a0 - a2*a1,
     *         a3*a0 - a2*a1,
     *         a0*a3 - a1*a2,
     *         a0*a3 - a1*a2}
     */
    __m256d r0_ = a;
    __m256d r1_ = simd256_reverse_(r0_);
    __m256d r2_ = _mm256_mul_pd(r0_, r1_);
    __m256d r3_ = _mm256_permute_pd(r2_, core_extension_(0b0110));

    return _mm256_hsub_pd(r3_, r3_);
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

#endif  /* CORE_GEOMETRY_SIMD_H_ */
