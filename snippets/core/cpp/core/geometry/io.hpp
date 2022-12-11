/*
 * io.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_IO_H_
#define CORE_GEOMETRY_IO_H_

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

#include "mat2.hpp"
#include "mat3.hpp"
#include "mat4.hpp"

namespace core {
namespace math {

/** ---------------------------------------------------------------------------
 * @brief Vector and matrix forward declarations.
 */
template<typename Type> struct vec2;
template<typename Type> struct vec3;
template<typename Type> struct vec4;
template<typename Type> struct mat2;
template<typename Type> struct mat3;
template<typename Type> struct mat4;

/** ---------------------------------------------------------------------------
 * @brief Vector and matrix output stream operators.
 */
template<typename Type>
core_inline_
std::ostream &operator<< (std::ostream &os, const vec2<Type> &v)
{
    os << v(0) << " " << v(1) << "\n";
    return os;
}

template<typename Type>
core_inline_
std::ostream &operator<< (std::ostream &os, const vec3<Type> &v)
{
    os << v(0) << " " << v(1) << " " << v(2) << "\n";
    return os;
}

template<typename Type>
core_inline_
std::ostream &operator<< (std::ostream &os, const vec4<Type> &v)
{
    os << v(0) << " " << v(1) << " " << v(2) << " " << v(3) << "\n";
    return os;
}

template<typename Type>
core_inline_
std::ostream &operator<< (std::ostream &os, const mat2<Type> &a)
{
    os << a(0,0) << " "
       << a(0,1) << "\n"

       << a(1,0) << " "
       << a(1,1) << "\n";
    return os;
}

template<typename Type>
core_inline_
std::ostream &operator<< (std::ostream &os, const mat3<Type> &a)
{
    os << a(0,0) << " "
       << a(0,1) << " "
       << a(0,2) << "\n"

       << a(1,0) << " "
       << a(1,1) << " "
       << a(1,2) << "\n"

       << a(2,0) << " "
       << a(2,1) << " "
       << a(2,2) << "\n";
    return os;
}

template<typename Type>
core_inline_
std::ostream &operator<< (std::ostream &os, const mat4<Type> &a)
{
    os << a(0,0) << " "
       << a(0,1) << " "
       << a(0,2) << " "
       << a(0,3) << "\n"

       << a(1,0) << " "
       << a(1,1) << " "
       << a(1,2) << " "
       << a(1,3) << "\n"

       << a(2,0) << " "
       << a(2,1) << " "
       << a(2,2) << " "
       << a(2,3) << "\n"

       << a(3,0) << " "
       << a(3,1) << " "
       << a(3,2) << " "
       << a(3,3) << "\n";
   return os;
}


/** ---------------------------------------------------------------------------
 * @brief Vector and matrix output string  functions.
 */
template<typename Type>
core_inline_
std::string to_string(const vec2<Type> &v, const char *format = nullptr)
{
    std::string write_format = (format != nullptr)
                             ? " " + std::string(format)
                             : " " + core::str::write_format<Type>();

    std::ostringstream ss;
    ss << core::str::format(write_format.c_str(), v(0));
    ss << core::str::format(write_format.c_str(), v(1));
    ss << core::str::format("\n");
    return ss.str();
}

template<typename Type>
core_inline_
std::string to_string(const vec3<Type> &v, const char *format = nullptr)
{
    std::string write_format = (format != nullptr)
                             ? " " + std::string(format)
                             : " " + core::str::write_format<Type>();

    std::ostringstream ss;
    ss << core::str::format(write_format.c_str(), v(0));
    ss << core::str::format(write_format.c_str(), v(1));
    ss << core::str::format(write_format.c_str(), v(2));
    ss << core::str::format("\n");
    return ss.str();
}

template<typename Type>
core_inline_
std::string to_string(const vec4<Type> &v, const char *format = nullptr)
{
    std::string write_format = (format != nullptr)
                             ? " " + std::string(format)
                             : " " + core::str::write_format<Type>();

    std::ostringstream ss;
    ss << core::str::format(write_format.c_str(), v(0));
    ss << core::str::format(write_format.c_str(), v(1));
    ss << core::str::format(write_format.c_str(), v(2));
    ss << core::str::format(write_format.c_str(), v(3));
    ss << core::str::format("\n");
    return ss.str();
}

template<typename Type>
core_inline_
std::string to_string(const mat2<Type> &a, const char *format = nullptr)
{
    std::string write_format = (format != nullptr)
                             ? " " + std::string(format)
                             : " " + core::str::write_format<Type>();

    std::ostringstream ss;
    ss << core::str::format(write_format.c_str(), a(0,0));
    ss << core::str::format(write_format.c_str(), a(0,1));
    ss << core::str::format("\n");
    ss << core::str::format(write_format.c_str(), a(1,0));
    ss << core::str::format(write_format.c_str(), a(1,1));
    ss << core::str::format("\n");
    return ss.str();
}

template<typename Type>
core_inline_
std::string to_string(const mat3<Type> &a, const char *format = nullptr)
{
    std::string write_format = (format != nullptr)
                             ? " " + std::string(format)
                             : " " + core::str::write_format<Type>();

    std::ostringstream ss;
    ss << core::str::format(write_format.c_str(), a(0,0));
    ss << core::str::format(write_format.c_str(), a(0,1));
    ss << core::str::format(write_format.c_str(), a(0,2));
    ss << core::str::format("\n");
    ss << core::str::format(write_format.c_str(), a(1,0));
    ss << core::str::format(write_format.c_str(), a(1,1));
    ss << core::str::format(write_format.c_str(), a(1,2));
    ss << core::str::format("\n");
    ss << core::str::format(write_format.c_str(), a(2,0));
    ss << core::str::format(write_format.c_str(), a(2,1));
    ss << core::str::format(write_format.c_str(), a(2,2));
    ss << core::str::format("\n");
    return ss.str();
}

template<typename Type>
core_inline_
std::string to_string(const mat4<Type> &a, const char *format = nullptr)
{
    std::string write_format = (format != nullptr)
                             ? " " + std::string(format)
                             : " " + core::str::write_format<Type>();

    std::ostringstream ss;
    ss << core::str::format(write_format.c_str(), a(0,0));
    ss << core::str::format(write_format.c_str(), a(0,1));
    ss << core::str::format(write_format.c_str(), a(0,2));
    ss << core::str::format(write_format.c_str(), a(0,3));
    ss << core::str::format("\n");
    ss << core::str::format(write_format.c_str(), a(1,0));
    ss << core::str::format(write_format.c_str(), a(1,1));
    ss << core::str::format(write_format.c_str(), a(1,2));
    ss << core::str::format(write_format.c_str(), a(1,3));
    ss << core::str::format("\n");
    ss << core::str::format(write_format.c_str(), a(2,0));
    ss << core::str::format(write_format.c_str(), a(2,1));
    ss << core::str::format(write_format.c_str(), a(2,2));
    ss << core::str::format(write_format.c_str(), a(2,3));
    ss << core::str::format("\n");
    ss << core::str::format(write_format.c_str(), a(3,0));
    ss << core::str::format(write_format.c_str(), a(3,1));
    ss << core::str::format(write_format.c_str(), a(3,2));
    ss << core::str::format(write_format.c_str(), a(3,3));
    ss << core::str::format("\n");
    return ss.str();
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

#endif /* CORE_GEOMETRY_IO_H_ */