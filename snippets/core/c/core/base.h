/*
 * base.h
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_BASE_H_
#define CORE_BASE_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/**
 * String macros
 */
#define core_str_(a)            #a
#define core_strify_(a)           core_str_(a)

/**
 * Minimum alignment in bytes for a variable or structure field.
 */
#ifndef core_cache_line_size_
#define core_cache_line_size_   (32)
#endif

/**
 * Define the SIMD AVX instruction set according to the __AVX__ macro,
 * set by mavx compiler option.
 *
 * SSE/AVX instructions operate on packed integers/floats/doubles, i.e.
 * Single Instruction Multiple Data (SIMD).
 * SSE vector registers are 128 bit wide (16 bytes), and able to pack
 * 4 integers (4 byte integer) or 2 doubles (8 byte double).
 * AVX vector registers are 256 bit wide (32 bytes), and able to pack
 * 8 integers (4 byte integer) or 4 doubles (8 byte double).
 *
 * Data is transferred in chuncks from/to vector registers. Data transfer
 * is more efficient if the memory addresses are a multiple of 16 bytes
 * (SSE) or 32 bytes (AVX).
 * A vector boundary, multiple of 16/32 bytes, is 16/32 byte aligned.
 *
 * Alternatively, fail with the error macro:
 *  #error AVX extended instruction set required.
 */
#ifdef CORE_USE_SIMD
#ifdef __AVX__
#include <x86intrin.h>
#else
#error AVX extended instruction set required.
#endif  /* __AVX__ */
#endif  /* CORE_USE_SIMD */

/**
 * POSIX standard symbolic constants, types and threads
 */
// #include <unistd.h>
// #include <pthread.h>

/**
 * OpenMP headers and macros
 *
 * These functions are declared in the omp.h header file and have well defined
 * meanings in a non parallel environment. If _OPENMP is not defined, fallback
 * and elide the macro.
 */
#ifdef _OPENMP
#include <omp.h>
#define core_omp_(a)        _Pragma(core_strify_(omp a))
#else
#define core_omp_(a)
inline int omp_get_level (void) { return 0; }
inline int omp_get_thread_num (void) { return 0; }
inline int omp_get_num_threads (void) { return 1; }
inline int omp_get_max_threads (void) { return 1; }
#endif  /* _OPENMP */

/**
 * GNU C extensions
 * @brief Convenience GNU-C extension macros.
 * Define the following extensions if we're using GNU-C.
 * Otherwise, fallback and elide the corresponding macro.
 * Alternatively, fail with:
 *  #error GNU C extensions required.
 */
#ifdef __GNUC__
#define core_inline_            inline __attribute__((always_inline))
#define core_aligned_           __attribute__((aligned(core_cache_line_size_)))
#define core_unused_(decl)      __attribute__((unused)) decl
#define core_extension_(decl)   __extension__ decl
#define core_restrict_(decl)    __restrict__ decl
#else
#define core_inline_
#define core_aligned_
#define core_unused_(decl)      decl
#define core_extension_(decl)   decl
#define core_restrict_(decl)    decl
#endif /* __GNUC__ */

/**
 * Throw and assert macros
 *
 * @note Enclose the compound statement inside a do {...} while(0)
 * to swallow the semicolon and the macro is one statement.
 */
#ifndef core_throw_
#define core_throw_(...) \
do { \
    fprintf( \
        stderr, \
        "runtime error: file %s, line %d: ", \
        __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    exit(EXIT_FAILURE); \
} while(0)
#endif /* core_throw_ */

#ifndef core_assert_
#define core_assert_(expr,...) \
do { \
    if (!(expr)) { \
        fprintf( \
            stderr, \
            "runtime error: file %s, line %d, %s: ", \
            __FILE__, \
            __LINE__, \
            core_strify_((expr))); \
        fprintf(stderr, __VA_ARGS__); \
        exit(EXIT_FAILURE); \
    } \
} while(0)
#endif /* core_assert_ */

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

#endif /* CORE_BASE_H_ */
