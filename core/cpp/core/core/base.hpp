/*
 * base.hpp
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

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <memory>

#include <vector>
#include <list>
#include <map>

#include <limits>

#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <exception>

/**
 * Common limits
 */
#ifndef core_true_
#define core_true_              (1)
#endif
#ifndef core_false_
#define core_false_             (0)
#endif
#ifndef core_float_eps_
#define core_float_eps_         (1.0e-6)
#endif
#ifndef core_double_eps_
#define core_double_eps_        (1.0e-15)
#endif
#ifndef core_long_double_eps_
#define core_long_double_eps_   (1.0e-33)
#endif
#ifndef core_name_max_
#define core_name_max_          (255)
#endif
#ifndef core_line_max_
#define core_line_max_          (2048)
#endif
#ifndef core_filename_max_
#define core_filename_max_      (1024)
#endif
#ifndef core_cache_line_size_
#define core_cache_line_size_   (32)
#endif

/**
 * Stringify macros
 */
#define core_str_(a)            #a
#define core_xstr_(a)           core_str_(a)

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
 * (SSE) or 32 bytes(AVX).
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
#define core_omp_(a)        _Pragma(core_xstr_(omp a))
#else
#define core_omp_(a)
inline int omp_get_level(void) { return 0; }
inline int omp_get_thread_num(void) { return 0; }
inline int omp_get_num_threads(void) { return 1; }
inline int omp_get_max_threads(void) { return 1; }
#endif  /* _OPENMP */

/**
 * GNU C extensions
 *
 * @brief Convenience GNU-C extension macros.
 * Define the following extensions if we're using GNU-C. Otherwise, fallback
 * and elide the corresponding macro. Alternatively, fail with:
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
#define core_unused_(decl)
#define core_extension_(decl)
#define core_restrict_(decl)
#endif /* __GNUC__ */

/*
 * Namespace core
 */
namespace core {

/** ---- Aligned data types ---------------------------------------------------
 *
 * @brief Define aligned data types to get the attribute to apply to the
 * pointed-to type, rather than the pointer itself.
 *
 * With GNU C __attribute__ syntax, it may be necessary use a typedef
 * to get the attribute to apply to the pointed-to type, rather than
 * to the pointer itself.
 *
 * @see
 * https://stackoverflow.com/questions/36211864
 * https://gcc.gnu.org/onlinedocs/gcc-6.5.0/gcc/Common-Type-Attributes.html
 */
typedef std::int8_t     aligned_int8_t     core_aligned_;
typedef std::uint8_t    aligned_uint8_t    core_aligned_;
typedef std::int16_t    aligned_int16_t    core_aligned_;
typedef std::uint16_t   aligned_uint16_t   core_aligned_;
typedef std::int32_t    aligned_int32_t    core_aligned_;
typedef std::uint32_t   aligned_uint32_t   core_aligned_;
typedef std::int64_t    aligned_int64_t    core_aligned_;
typedef std::uint64_t   aligned_uint64_t   core_aligned_;
typedef std::float_t    aligned_float_t    core_aligned_;
typedef std::double_t   aligned_double_t   core_aligned_;

/** ---- Stringify function ---------------------------------------------------
 *
 * strify<...>
 *
 * @brief Create a single string represention of all the arguments.
 * Use a variadic template to iterate over each argument and construct the
 * concatenated string recursively.
 *
 * @see
 * https://en.cppreference.com/w/cpp/language/parameter_pack
 * https://eli.thegreenplace.net/2014/variadic-templates-in-c/
 * https://stackoverflow.com/questions/54744252
 */
template<typename... Args>
core_inline_
std::string strify(Args&&... args);

template<typename T, typename... Args>
core_inline_
std::string strify(T t, Args&&... args)
{
    std::ostringstream ss;
    ss << t << strify(static_cast<Args&&>(args)...);
    return ss.str();
}

template<>
core_inline_
std::string strify()
{
    return std::string();
}

}  /* namespace core */

/** ---- Throw and assert macros ----------------------------------------------
 *
 * @brief These macros encapsulate debug information about a runtime error -
 * current file, current line number, name of the called function and error
 * message.
 * Alternatively, inherit from std::runtime class and create a custom
 * exception handler.
 *
 * @see
 * https://en.cppreference.com/w/cpp/error/exception
 * https://en.cppreference.com/w/cpp/error/runtime_error
 * https://stackoverflow.com/questions/8152720
 * https://stackoverflow.com/questions/1569726
 *
 * @note Enclose the compound statement inside a do {...} while(0)
 * to swallow the semicolon and the macro in one statement.
 */
#ifndef core_error_
#define core_error_(message) \
do { \
    std::cerr << core::strify( \
        "file ", __FILE__, \
         ", line ", __LINE__, \
         ": ", (message), \
         "\n"); \
} while(0)
#endif /* core_error_ */

#ifndef core_throw_
#define core_throw_(message) \
do { \
    throw std::runtime_error(core::strify( \
        "file ", __FILE__, \
        ", line ", __LINE__, \
        ": ", (message), \
        "\n")); \
} while(0)
#endif /* core_throw_ */

#ifndef core_assert_
#define core_assert_(expr,message) \
do { \
    if (!(expr)) { \
        throw std::runtime_error(core::strify( \
            "file ", __FILE__, \
            ", line ", __LINE__, \
            ", ", core_xstr_((expr)), \
            ": ", (message), \
            "\n")); \
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
