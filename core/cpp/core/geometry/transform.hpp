/*
 * transform.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_TRANSFORM_H_
#define CORE_GEOMETRY_TRANSFORM_H_

#include "vec3.hpp"
#include "mat4.hpp"

namespace core {
namespace math {

/** ---------------------------------------------------------------------------
 * translate
 * @brief Translate this matrix by d.
 */
template<typename Type>
core_inline_
mat4<Type> translate(const vec3<Type> d)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    const mat4<Type> result(
        1.0, 0.0, 0.0, d(0),
        0.0, 1.0, 0.0, d(1),
        0.0, 0.0, 1.0, d(2),
        0.0, 0.0, 0.0, 1.0);
    return result;
}

template<typename Type>
core_inline_
mat4<Type> translate(const mat4<Type> a, const vec3<Type> d)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (translate(d) * a);
}


/** ---------------------------------------------------------------------------
 * rotate
 * @brief Rotate this matrix by theta given an axis of rotation defined by n.
 */
template<typename Type>
core_inline_
mat4<Type> rotate(vec3<Type> n, const Type theta)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    /* Compute trigonometric coefficients */
    const Type alpha = (1.0 - std::cos(theta));
    const Type beta = std::cos(theta);
    const Type gamma = std::sin(theta);

    /* Compute diadic product matrix */
    n = normalize(n);
    mat4<Type> Rd;
    Rd = identity(Rd);
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            Rd(i,j) = n(i) * n(j);
        }
    }

    /* Compute identity matrix */
    mat4<Type> I;
    I = identity(I);

    /* Compute cross product matrix */
    mat4<Type> Rc;
    Rc = identity(Rc);
    Rc(0,0) =  0.0;
    Rc(0,1) = -n(2);
    Rc(0,2) =  n(1);

    Rc(1,0) =  n(2);
    Rc(1,1) =  0.0;
    Rc(1,2) = -n(0);

    Rc(2,0) = -n(1);
    Rc(2,1) =  n(0);
    Rc(2,2) =  0.0;

    /* Compute rotation matrix */
    mat4<Type> R;
    R = identity(R);
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            R(i,j) = alpha*Rd(i,j) + beta*I(i,j) + gamma*Rc(i,j);
        }
    }
}

template<typename Type>
core_inline_
mat4<Type> rotate(const mat4<Type> a, vec3<Type> n, const Type theta)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (rotate(n, theta) * a);
}


/** ---------------------------------------------------------------------------
 * lookat
 * @brief Compute a lookat local frame of reference given the eye position,
 * the ctr reference viewpoint and the updward directon vector up.
 *
 * The convention used here is one of a right-handed local basis set
 * where the z-axis is represented by the forward unit f-vector, with
 * origin at the eye and direction towards the ctr view position.
 * The x-axis is represented by the sideway unit s-vector and the y-axis
 * is represented by the upward unit u-vector in the local reference frame.
 */
template<typename Type>
core_inline_
mat4<Type> lookat(
    const vec3<Type> &eye,
    const vec3<Type> &ctr,
    const vec3<Type> &up)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    /*
     * Compute the lookat local basis set s-u-f.
     */
    vec3<Type> f = normalize(ctr - eye);    /* f = ctr - eye */
    vec3<Type> s = normalize(cross(f, up)); /* s = f x up */
    vec3<Type> u = normalize(cross(s, f));  /* u = s x f */

    /*
     * Compute the lookat projection matrix and* corresponding
     * eye translation position.
     */
    mat4<Type> M(
         s(0),  s(1),  s(2), -eye(0),
         u(0),  u(1),  u(2), -eye(1),
        -f(0), -f(1), -f(2), -eye(2),
         0.0,   0.0,   0.0,    1.0);
    return M;
}

template<typename Type>
core_inline_
mat4<Type> lookat(
    const mat4<Type> a,
    const vec3<Type> &eye,
    const vec3<Type> &ctr,
    const vec3<Type> &up)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (lookat(eye, ctr, up) * a);
}


/** ---------------------------------------------------------------------------
 * perspective
 * @brief Compute the perspective projection matrix.
 */
template<typename Type>
core_inline_
mat4<Type> perspective(
    const Type fovy,
    const Type aspect,
    const Type znear,
    const Type zfar)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    const Type ftan = 1.0 / (Type) std::tan(fovy/2.0);

    mat4<Type> result;
    result(0,0) =  ftan / aspect;
    result(0,1) =  0.0;
    result(0,2) =  0.0;
    result(0,3) =  0.0;

    result(1,0) =  0.0;
    result(1,1) =  ftan;
    result(1,2) =  0.0;
    result(1,3) =  0.0;

    result(2,0) =  0.0;
    result(2,1) =  0.0;
    result(2,2) = -(zfar+znear) / (zfar-znear);
    result(2,3) = -(2.0*zfar*znear) / (zfar-znear);

    result(3,0) =  0.0;
    result(3,1) =  0.0;
    result(3,2) = -1.0;
    result(3,3) =  1.0;
    return result;
}

template<typename Type>
core_inline_
mat4<Type> perspective(
    const mat4<Type> a,
    const Type fovy,
    const Type aspect,
    const Type znear,
    const Type zfar)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (perspective(fovy, aspect, znear, zfar) * a);
}


/** ---------------------------------------------------------------------------
 * ortho
 * @brief Compute the orthographic projection matrix.
 */
template<typename Type>
core_inline_
mat4<Type> ortho(
    const Type left,
    const Type right,
    const Type bottom,
    const Type top,
    const Type znear,
    const Type zfar)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");

    mat4<Type> result;
    result(0,0) =  2.0 / (right-left);
    result(0,1) =  0.0;
    result(0,2) =  0.0;
    result(0,3) =  -(right+left) / (right-left);

    result(1,0) =  0.0;
    result(1,1) =  2.0 / (top-bottom);
    result(1,2) =  0.0;
    result(1,3) =  -(top+bottom) / (top-bottom);

    result(2,0) =  0.0;
    result(2,1) =  0.0;
    result(2,2) = -2.0 / (zfar-znear);
    result(2,3) = -(zfar+znear) / (zfar-znear);

    result(3,0) =  0.0;
    result(3,1) =  0.0;
    result(3,2) =  0.0;
    result(3,3) =  1.0;
    return result;
}

template<typename Type>
core_inline_
mat4<Type> ortho(
    const mat4<Type> a,
    const Type left,
    const Type right,
    const Type bottom,
    const Type top,
    const Type znear,
    const Type zfar)
{
    static_assert(std::is_floating_point<Type>::value, "non floating point");
    return (ortho(left, right, bottom, top, znear, zfar) * a);
}

} /* namespace math */
} /* namespace core */


/** ---------------------------------------------------------------------------
 * @brief Enable simd vectorized instructions.
 */
#ifdef CORE_USE_SIMD
#include "transform-simd.hpp"
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

#endif /* CORE_GEOMETRY_TRANSFORM_H_ */
