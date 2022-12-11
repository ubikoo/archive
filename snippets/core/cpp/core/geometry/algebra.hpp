/*
 * algebra.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_ALGEBRA_H_
#define CORE_GEOMETRY_ALGEBRA_H_

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

#include "mat2.hpp"
#include "mat3.hpp"
#include "mat4.hpp"

namespace core {
namespace math {

/** ---------------------------------------------------------------------------
 * dot
 * @brief Return the dot product of two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/dot.xhtml
 */
template<typename Type>
core_inline_
Type dot(const vec2<Type> &a, const vec2<Type> &b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (a(0) * b(0) +
            a(1) * b(1));
}

template<typename Type>
core_inline_
Type dot(const vec3<Type> &a, const vec3<Type> &b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (a(0) * b(0) +
            a(1) * b(1) +
            a(2) * b(2));
}

template<typename Type>
core_inline_
Type dot(const vec4<Type> &a, const vec4<Type> &b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (a(0) * b(0) +
            a(1) * b(1) +
            a(2) * b(2) +
            a(3) * b(3));
}


/** ---------------------------------------------------------------------------
 * norm
 * @brief Return the norm product of two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/length.xhtml
 */
template<typename Type>
core_inline_
Type norm(const vec2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return std::sqrt(dot(a,a));
}

template<typename Type>
core_inline_
Type norm(const vec3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return std::sqrt(dot(a,a));
}

template<typename Type>
core_inline_
Type norm(const vec4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return std::sqrt(dot(a,a));
}


/** ---------------------------------------------------------------------------
 * normalize
 * @brief Return the normalize product of two vectors.
 * @see
 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/normalize.xhtml
 */
template<typename Type>
core_inline_
vec2<Type> normalize(const vec2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (a / norm(a));
}

template<typename Type>
core_inline_
vec3<Type> normalize(const vec3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (a / norm(a));
}

template<typename Type>
core_inline_
vec4<Type> normalize(const vec4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (a / norm(a));
}


/** ---------------------------------------------------------------------------
 * distance
 * @brief Return the distance product of two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/distance.xhtml
 */
template<typename Type>
core_inline_
Type distance(const vec2<Type> &a, const vec2<Type> &b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return norm(a-b);
}

template<typename Type>
core_inline_
Type distance(const vec3<Type> &a, const vec3<Type> &b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return norm(a-b);
}

template<typename Type>
core_inline_
Type distance(const vec4<Type> &a, const vec4<Type> &b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return norm(a-b);
}


/** ---------------------------------------------------------------------------
 * cross
 * @brief Return the cross product of two vectors.
 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/cross.xhtml
 */
template<typename Type>
core_inline_
vec3<Type> cross(const vec3<Type> &a, const vec3<Type> &b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return vec3<Type>(
        a(1)*b(2) - a(2)*b(1),
        a(2)*b(0) - a(0)*b(2),
        a(0)*b(1) - a(1)*b(0));
}


/** ---------------------------------------------------------------------------
 * identity
 * @brief Return the identity matrix.
 */
template<typename Type>
core_inline_
mat2<Type> identity(const mat2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    constexpr Type zero = (Type) 0;
    constexpr Type one = (Type) 1;
    mat2<Type> eye(
        one, zero,
        zero, one);
    return eye;
}

template<typename Type>
core_inline_
mat3<Type> identity(const mat3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    constexpr Type zero = (Type) 0;
    constexpr Type one = (Type) 1;
    mat3<Type> eye(
        one, zero, zero,
        zero, one, zero,
        zero, zero, one);
    return eye;
}

template<typename Type>
core_inline_
mat4<Type> identity(const mat4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    constexpr Type zero = (Type) 0;
    constexpr Type one = (Type) 1;
    mat4<Type> eye(
        one, zero, zero, zero,
        zero, one, zero, zero,
        zero, zero, one, zero,
        zero, zero, zero, one);
    return eye;
}


/** ---------------------------------------------------------------------------
 * transpose
 * @brief Return the transpose of the matrix.
 */
template<typename Type>
core_inline_
mat2<Type> transpose(const mat2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return mat2<Type>(
        a(0,0), a(1,0),
        a(0,1), a(1,1));
}

template<typename Type>
core_inline_
mat3<Type> transpose(const mat3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return mat3<Type>(
        a(0,0), a(1,0), a(2,0),
        a(0,1), a(1,1), a(2,1),
        a(0,2), a(1,2), a(2,2));
}

template<typename Type>
core_inline_
mat4<Type> transpose(const mat4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return mat4<Type>(
        a(0,0), a(1,0), a(2,0), a(3,0),
        a(0,1), a(1,1), a(2,1), a(3,1),
        a(0,2), a(1,2), a(2,2), a(3,2),
        a(0,3), a(1,3), a(2,3), a(3,3));
}


/** ---------------------------------------------------------------------------
 * determinant
 * @brief Return the determinant of the matrix.
 */
template<typename Type>
core_inline_
Type determinant(const mat2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    /*
     * M = {m0, m1,
     *      m2, m3}
     *
     * det(M) = +m0 * m3
     *          -m1 * m2
     */
    Type det = a(0,0)*a(1,1) - a(0,1)*a(1,0);
    return det;
}

template<typename Type>
core_inline_
Type determinant(const mat3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    /*
     * M = {m0, m1, m2,
     *      m3, m4, m5,
     *      m6, m7, m8}
     *
     * det(M) = +m0 * m4 * m8
     *          -m0 * m5 * m7
     *          -m1 * m3 * m8
     *          +m1 * m5 * m6
     *          +m2 * m3 * m7
     *          -m2 * m4 * m6
     */
    Type minor0 = a(1,1)*a(2,2) - a(1,2)*a(2,1);
    Type minor1 = a(1,2)*a(2,0) - a(1,0)*a(2,2);
    Type minor2 = a(1,0)*a(2,1) - a(1,1)*a(2,0);
    Type det = a(0,0)*minor0 + a(0,1)*minor1 + a(0,2)*minor2;
    return det;
}

template<typename Type>
core_inline_
Type determinant(const mat4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    /*
     * M = {m0,  m1,  m2,  m3,
     *      m4,  m5,  m6,  m7,
     *      m8,  m9,  m10, m11,
     *      m12, m13, m14, m15}
     *
     * det(M) = -m0  * m10 * m13 * m7
     *          +m0  * m10 * m15 * m5
     *          +m0  * m11 * m13 * m6
     *          -m0  * m11 * m14 * m5
     *          +m0  * m14 * m7  * m9
     *          -m0  * m15 * m6  * m9
     *          +m1  * m10 * m12 * m7
     *          -m1  * m10 * m15 * m4
     *          -m1  * m11 * m12 * m6
     *          +m1  * m11 * m14 * m4
     *          -m1  * m14 * m7  * m8
     *          +m1  * m15 * m6  * m8
     *          -m10 * m12 * m3  * m5
     *          +m10 * m13 * m3  * m4
     *          +m11 * m12 * m2  * m5
     *          -m11 * m13 * m2  * m4
     *          -m12 * m2  * m7  * m9
     *          +m12 * m3  * m6  * m9
     *          +m13 * m2  * m7  * m8
     *          -m13 * m3  * m6  * m8
     *          -m14 * m3  * m4  * m9
     *          +m14 * m3  * m5  * m8
     *          +m15 * m2  * m4  * m9
     *          -m15 * m2  * m5  * m8
    */
    Type minor0  = a(2,2)*a(3,3) - a(2,3)*a(3,2);
    Type minor1  = a(2,3)*a(3,1) - a(2,1)*a(3,3);
    Type minor2  = a(2,1)*a(3,2) - a(2,2)*a(3,1);

    Type minor3  = a(2,3)*a(3,2) - a(2,2)*a(3,3);
    Type minor4  = a(2,0)*a(3,3) - a(2,3)*a(3,0);
    Type minor5  = a(2,2)*a(3,0) - a(2,0)*a(3,2);

    Type minor6  = a(2,1)*a(3,3) - a(2,3)*a(3,1);
    Type minor7  = a(2,3)*a(3,0) - a(2,0)*a(3,3);
    Type minor8  = a(2,0)*a(3,1) - a(2,1)*a(3,0);

    Type minor9  = a(2,2)*a(3,1) - a(2,1)*a(3,2);
    Type minor10 = a(2,0)*a(3,2) - a(2,2)*a(3,0);
    Type minor11 = a(2,1)*a(3,0) - a(2,0)*a(3,1);

    Type det = a(0,0) * (a(1,1)*minor0 + a(1,2)*minor1  + a(1,3)*minor2) +
               a(0,1) * (a(1,0)*minor3 + a(1,2)*minor4  + a(1,3)*minor5) +
               a(0,2) * (a(1,0)*minor6 + a(1,1)*minor7  + a(1,3)*minor8) +
               a(0,3) * (a(1,0)*minor9 + a(1,1)*minor10 + a(1,2)*minor11);
    return det;
}


/** ---------------------------------------------------------------------------
 * inverse
 * @brief Return the inverse of the matrix.
 */
template<typename Type>
core_inline_
mat2<Type> inverse(const mat2<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    /*
     * Compute the inverse matrix from the corresponding adjugate:
     * inv(M) = adj(M) / det(M)
     * adj(M) = { m3, -m1,
     *           -m2,  m0}
     */
    mat2<Type> adj(
         a(1,1), -a(0,1),
        -a(1,0),  a(0,0));

    /* Compute determinant from Laplace's expansion */
    double det = a(0,0)*adj(0,0) + a(0,1)*adj(1,0);
    det = std::fabs(det) > 0.0 ? 1.0 / det : 0.0;

    return (adj *= det);
}

template<typename Type>
core_inline_
mat3<Type> inverse(const mat3<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    /*
     * Compute the inverse matrix from the corresponding adjugate:
     * inv(M) = adj(M) / det(M)
     * adj(M) = {+m4*m8 - m5*m7,
     *           +m2*m7 - m1*m8,
     *           +m1*m5 - m2*m4
     *           +m5*m6 - m3*m8,
     *           +m0*m8 - m2*m6,
     *           +m2*m3 - m0*m5,
     *           +m3*m7 - m4*m6,
     *           +m1*m6 - m0*m7,
     *           +m0*m4 - m1*m3}
     */
    mat3<Type> adj(
        a(1,1)*a(2,2) - a(1,2)*a(2,1),
        a(0,2)*a(2,1) - a(0,1)*a(2,2),
        a(0,1)*a(1,2) - a(0,2)*a(1,1),

        a(1,2)*a(2,0) - a(1,0)*a(2,2),
        a(0,0)*a(2,2) - a(0,2)*a(2,0),
        a(0,2)*a(1,0) - a(0,0)*a(1,2),

        a(1,0)*a(2,1) - a(1,1)*a(2,0),
        a(0,1)*a(2,0) - a(0,0)*a(2,1),
        a(0,0)*a(1,1) - a(0,1)*a(1,0));

    /* Compute determinant from Laplace's expansion */
    double det = a(0,0) * adj(0,0) +
                 a(0,1) * adj(1,0) +
                 a(0,2) * adj(2,0);
    det = std::fabs(det) > 0.0 ? 1.0 / det : 0.0;

    return (adj *= det);
}

template<typename Type>
core_inline_
mat4<Type> inverse(const mat4<Type> &a)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    /*
     * Compute the inverse matrix from the corresponding adjugate:
     * inv(M) = adj(M) / det(M)
     * adj(M) =
     * {
     *      -m11*m14*m5  +m10*m15*m5  +m11*m13*m6
     *      -m10*m13*m7  -m15*m6 *m9  +m14*m7 *m9,
     *
     *      +m1 *m11*m14 -m3 *m9 *m14 -m1 *m10*m15
     *      -m11*m13*m2  +m10*m13*m3  +m15*m2 *m9,
     *
     *      -m15*m2 *m5  +m14*m3 *m5  +m1 *m15*m6
     *      -m13*m3 *m6  -m1 *m14*m7  +m13*m2 *m7,
     *
     *      +m11*m2 *m5  -m10*m3 *m5  -m1 *m11*m6
     *      +m1 *m10*m7  +m3 *m6 *m9  -m2 *m7 *m9,
     *
     *      +m11*m14*m4  -m10*m15*m4  -m11*m12*m6
     *      +m10*m12*m7  +m15*m6 *m8  -m14*m7 *m8,
     *
     *      -m0 *m11*m14 +m3 *m8 *m14 +m0 *m10*m15
     *      +m11*m12*m2  -m10*m12*m3  -m15*m2 *m8,
     *
     *      +m15*m2 *m4  -m14*m3 *m4  -m0 *m15*m6
     *      +m12*m3 *m6  +m0 *m14*m7  -m12*m2 *m7,
     *
     *      -m11*m2 *m4  +m10*m3 *m4  +m0 *m11*m6
     *      -m0 *m10*m7  -m3 *m6 *m8  +m2 *m7 *m8,
     *
     *      -m11*m13*m4  +m15*m9 *m4  +m11*m12*m5
     *      -m15*m5 *m8  +m13*m7 *m8  -m12*m7 *m9,
     *
     *      -m1 *m11*m12 +m3 *m9 *m12 +m0 *m11*m13
     *      +m1 *m15*m8  -m13*m3 *m8  -m0 *m15*m9,
     *
     *      -m1 *m15*m4  +m13*m3 *m4  +m0 *m15*m5
     *      -m12*m3 *m5  +m1 *m12*m7  -m0 *m13*m7,
     *
     *      +m1 *m11*m4  -m3 *m9 *m4  -m0 *m11*m5
     *      +m3 *m5 *m8  -m1 *m7 *m8  +m0 *m7 *m9,
     *
     *      +m10*m13*m4  -m14*m9 *m4  -m10*m12*m5
     *      +m14*m5 *m8  -m13*m6 *m8  +m12*m6 *m9,
     *
     *      +m1 *m10*m12 -m2 *m9 *m12 -m0 *m10*m13
     *      -m1 *m14*m8  +m13*m2 *m8  +m0 *m14*m9,
     *
     *      +m1 *m14*m4  -m13*m2 *m4  -m0 *m14*m5
     *      +m12*m2 *m5  -m1 *m12*m6  +m0 *m13*m6,
     *
     *      -m1 *m10*m4  +m2 *m9 *m4  +m0 *m10*m5
     *      -m2 *m5 *m8  +m1 *m6 *m8  -m0 *m6 *m9
     * }
     */
    mat4<Type> adj(
        a(1,1) * (a(2,2)*a(3,3) - a(2,3)*a(3,2)) +
        a(1,2) * (a(2,3)*a(3,1) - a(2,1)*a(3,3)) +
        a(1,3) * (a(2,1)*a(3,2) - a(2,2)*a(3,1)),

        a(0,1) * (a(2,3)*a(3,2) - a(2,2)*a(3,3)) +
        a(0,2) * (a(2,1)*a(3,3) - a(2,3)*a(3,1)) +
        a(0,3) * (a(2,2)*a(3,1) - a(2,1)*a(3,2)),

        a(1,1) * (a(0,3)*a(3,2) - a(0,2)*a(3,3)) +
        a(1,2) * (a(0,1)*a(3,3) - a(0,3)*a(3,1)) +
        a(1,3) * (a(0,2)*a(3,1) - a(0,1)*a(3,2)),

        a(1,1) * (a(0,2)*a(2,3) - a(2,2)*a(0,3)) +
        a(1,2) * (a(0,3)*a(2,1) - a(0,1)*a(2,3)) +
        a(1,3) * (a(0,1)*a(2,2) - a(0,2)*a(2,1)),

        a(1,0) * (a(2,3)*a(3,2) - a(2,2)*a(3,3)) +
        a(1,2) * (a(2,0)*a(3,3) - a(2,3)*a(3,0)) +
        a(1,3) * (a(2,2)*a(3,0) - a(2,0)*a(3,2)),

        a(0,0) * (a(2,2)*a(3,3) - a(2,3)*a(3,2)) +
        a(0,2) * (a(2,3)*a(3,0) - a(2,0)*a(3,3)) +
        a(0,3) * (a(2,0)*a(3,2) - a(2,2)*a(3,0)),

        a(0,0) * (a(1,3)*a(3,2) - a(1,2)*a(3,3)) +
        a(0,2) * (a(1,0)*a(3,3) - a(1,3)*a(3,0)) +
        a(0,3) * (a(1,2)*a(3,0) - a(1,0)*a(3,2)),

        a(0,0) * (a(1,2)*a(2,3) - a(1,3)*a(2,2)) +
        a(0,2) * (a(1,3)*a(2,0) - a(1,0)*a(2,3)) +
        a(0,3) * (a(1,0)*a(2,2) - a(1,2)*a(2,0)),

        a(1,0) * (a(2,1)*a(3,3) - a(2,3)*a(3,1)) +
        a(1,1) * (a(2,3)*a(3,0) - a(2,0)*a(3,3)) +
        a(1,3) * (a(2,0)*a(3,1) - a(2,1)*a(3,0)),

        a(0,0) * (a(2,3)*a(3,1) - a(2,1)*a(3,3)) +
        a(0,1) * (a(2,0)*a(3,3) - a(2,3)*a(3,0)) +
        a(0,3) * (a(2,1)*a(3,0) - a(2,0)*a(3,1)),

        a(0,0) * (a(1,1)*a(3,3) - a(1,3)*a(3,1)) +
        a(0,1) * (a(1,3)*a(3,0) - a(1,0)*a(3,3)) +
        a(0,3) * (a(1,0)*a(3,1) - a(1,1)*a(3,0)),

        a(0,0) * (a(1,3)*a(2,1) - a(1,1)*a(2,3)) +
        a(0,1) * (a(1,0)*a(2,3) - a(1,3)*a(2,0)) +
        a(0,3) * (a(1,1)*a(2,0) - a(2,1)*a(1,0)),

        a(1,0) * (a(2,2)*a(3,1) - a(2,1)*a(3,2)) +
        a(1,1) * (a(2,0)*a(3,2) - a(2,2)*a(3,0)) +
        a(1,2) * (a(2,1)*a(3,0) - a(2,0)*a(3,1)),

        a(0,0) * (a(2,1)*a(3,2) - a(2,2)*a(3,1)) +
        a(0,1) * (a(2,2)*a(3,0) - a(2,0)*a(3,2)) +
        a(0,2) * (a(2,0)*a(3,1) - a(2,1)*a(3,0)),

        a(0,0) * (a(1,2)*a(3,1) - a(1,1)*a(3,2)) +
        a(0,1) * (a(1,0)*a(3,2) - a(1,2)*a(3,0)) +
        a(0,2) * (a(1,1)*a(3,0) - a(1,0)*a(3,1)),

        a(0,0) * (a(1,1)*a(2,2) - a(1,2)*a(2,1)) +
        a(0,1) * (a(1,2)*a(2,0) - a(1,0)*a(2,2)) +
        a(0,2) * (a(1,0)*a(2,1) - a(1,1)*a(2,0)));

    /* Compute inverse matrix using Laplace's expansion */
    double det = a(0,0) * adj(0,0) +
                 a(0,1) * adj(1,0) +
                 a(0,2) * adj(2,0) +
                 a(0,3) * adj(3,0);
    det = std::fabs(det) > 0.0 ? 1.0 / det : 0.0;

    return (adj *= det);
}


/** ---------------------------------------------------------------------------
 * multiply
 * @brief Return the multiply of the matrix.
 */
template<typename Type>
core_inline_
vec2<Type> multiply(const mat2<Type> &a, const vec2<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec2<Type> result(0.0);
    for (size_t i = 0; i < a.dim(); ++i) {
        for (size_t j = 0; j < a.dim(); ++j) {
            result(i) += a(i,j) * v(j);
        }
    }
    return result;
}

template<typename Type>
core_inline_
mat2<Type> multiply(const mat2<Type> &a, const mat2<Type> &b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat2<Type> result(0.0);
    for (size_t i = 0; i < a.dim(); ++i) {
        for (size_t j = 0; j < a.dim(); ++j) {
            for (size_t k = 0; k < a.dim(); ++k) {
                result(i,j) += a(i,k) * b(k,j);
            }
        }
    }
    return result;
}

template<typename Type>
core_inline_
vec3<Type> multiply(const mat3<Type> &a, const vec3<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec3<Type> result(0.0);
    for (size_t i = 0; i < a.dim(); ++i) {
        for (size_t j = 0; j < a.dim(); ++j) {
            result(i) += a(i,j) * v(j);
        }
    }
    return result;
}

template<typename Type>
core_inline_
mat3<Type> multiply(const mat3<Type> &a, const mat3<Type> &b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat3<Type> result(0.0);
    for (size_t i = 0; i < a.dim(); ++i) {
        for (size_t j = 0; j < a.dim(); ++j) {
            for (size_t k = 0; k < a.dim(); ++k) {
                result(i,j) += a(i,k) * b(k,j);
            }
        }
    }
    return result;
}


template<typename Type>
core_inline_
vec4<Type> multiply(const mat4<Type> &a, const vec4<Type> &v)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    vec4<Type> result(0.0);
    for (size_t i = 0; i < a.dim(); ++i) {
        for (size_t j = 0; j < a.dim(); ++j) {
            result(i) += a(i,j) * v(j);
        }
    }
    return result;
}

template<typename Type>
core_inline_
mat4<Type> multiply(const mat4<Type> &a, const mat4<Type> &b)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat4<Type> result(0.0);
    for (size_t i = 0; i < a.dim(); ++i) {
        for (size_t j = 0; j < a.dim(); ++j) {
            for (size_t k = 0; k < a.dim(); ++k) {
                result(i,j) += a(i,k) * b(k,j);
            }
        }
    }
    return result;
}

} /* namespace math */
} /* namespace core */


/** ---------------------------------------------------------------------------
 * @brief Enable simd vectorized instructions.
 */
#ifdef CORE_USE_SIMD
#include "algebra-simd.hpp"
#endif

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

#endif /* CORE_GEOMETRY_ALGEBRA */
