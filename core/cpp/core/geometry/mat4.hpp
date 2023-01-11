/*
 * mat4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_MAT4_H_
#define CORE_GEOMETRY_MAT4_H_

namespace core {
namespace math {

/** ---- mat4 class -----------------------------------------------------------
 *
 * struct mat4
 * @brief
 * mat4 is a plain class representing a 2x2-component matrix of type double.
 */
template<typename Type>
struct mat4 {
    /* Types */
    typedef std::size_t     size_type;
    typedef Type            value_type;
    typedef Type&           reference;
    typedef const Type&     const_reference;

    /* Static constants */
    static const size_type dim_ = 4;
    static const size_type length_ = dim_*dim_;

    /* Member variables/accessors */
    value_type data_[dim_][dim_] core_aligned_;

    /* Capacity */
    size_type dim(void) const { return dim_; }
    size_type length(void) const { return length_; }
    size_type size(void) const { return sizeof(data_); }

    /* Element access */
    value_type *data(void) { return &data_[0][0]; }
    const value_type *data(void) const { return &data_[0][0]; }

    value_type &operator()(size_t i, size_t j) { return data_[i][j]; }
    const value_type &operator()(size_t i, size_t j) const {
        return data_[i][j];
    }

    /* Unary arithmetic vector operators. */
    mat4 &operator+=(const mat4 &other);
    mat4 &operator-=(const mat4 &other);
    mat4 &operator*=(const mat4 &other);
    mat4 &operator/=(const mat4 &other);

    /* Unary arithmetic scalar operators. */
    mat4 &operator+=(const Type scalar);
    mat4 &operator-=(const Type scalar);
    mat4 &operator*=(const Type scalar);
    mat4 &operator/=(const Type scalar);

    /* Increment operators. */
    mat4 &operator++ (void);
    mat4 &operator-- (void);
    mat4 operator++ (int);
    mat4 operator-- (int);

    /* Constructor/destructor. */
    mat4 () { std::memset(data_, 0, sizeof(data_)); }
    explicit mat4 (const value_type *data) {
        std::memcpy(data_, data, sizeof(data_));
    }
    explicit mat4 (const Type e0) {
        data_[0][0] = e0;
        data_[0][1] = e0;
        data_[0][2] = e0;
        data_[0][3] = e0;

        data_[1][0] = e0;
        data_[1][1] = e0;
        data_[1][2] = e0;
        data_[1][3] = e0;

        data_[2][0] = e0;
        data_[2][1] = e0;
        data_[2][2] = e0;
        data_[2][3] = e0;

        data_[3][0] = e0;
        data_[3][1] = e0;
        data_[3][2] = e0;
        data_[3][3] = e0;
    }
    explicit mat4 (
        const Type e0,
        const Type e1,
        const Type e2,
        const Type e3,
        const Type e4,
        const Type e5,
        const Type e6,
        const Type e7,
        const Type e8,
        const Type e9,
        const Type e10,
        const Type e11,
        const Type e12,
        const Type e13,
        const Type e14,
        const Type e15) {
        data_[0][0] = e0;
        data_[0][1] = e1;
        data_[0][2] = e2;
        data_[0][3] = e3;

        data_[1][0] = e4;
        data_[1][1] = e5;
        data_[1][2] = e6;
        data_[1][3] = e7;

        data_[2][0] = e8;
        data_[2][1] = e9;
        data_[2][2] = e10;
        data_[2][3] = e11;

        data_[3][0] = e12;
        data_[3][1] = e13;
        data_[3][2] = e14;
        data_[3][3] = e15;
    }
    ~mat4 () = default;

    /* Copy constructor/assignment. */
    mat4 (const mat4 &other) {
        std::memcpy(data_, other.data_, sizeof(data_));
    }
    mat4 &operator=(const mat4 &other) {
        if (this == &other) {
            return *this;
        }
        std::memcpy(data_, other.data_, sizeof(data_));
        return *this;
    }
}; /* struct mat4 */


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
template<typename Type>
core_inline_
mat4<Type> &mat4<Type>::operator+=(const mat4<Type> &other)
{
    data_[0][0] += other.data_[0][0];
    data_[0][1] += other.data_[0][1];
    data_[0][2] += other.data_[0][2];
    data_[0][3] += other.data_[0][3];

    data_[1][0] += other.data_[1][0];
    data_[1][1] += other.data_[1][1];
    data_[1][2] += other.data_[1][2];
    data_[1][3] += other.data_[1][3];

    data_[2][0] += other.data_[2][0];
    data_[2][1] += other.data_[2][1];
    data_[2][2] += other.data_[2][2];
    data_[2][3] += other.data_[2][3];

    data_[3][0] += other.data_[3][0];
    data_[3][1] += other.data_[3][1];
    data_[3][2] += other.data_[3][2];
    data_[3][3] += other.data_[3][3];
    return *this;
}

template<typename Type>
core_inline_
mat4<Type> &mat4<Type>::operator-=(const mat4<Type> &other)
{
    data_[0][0] -= other.data_[0][0];
    data_[0][1] -= other.data_[0][1];
    data_[0][2] -= other.data_[0][2];
    data_[0][3] -= other.data_[0][3];

    data_[1][0] -= other.data_[1][0];
    data_[1][1] -= other.data_[1][1];
    data_[1][2] -= other.data_[1][2];
    data_[1][3] -= other.data_[1][3];

    data_[2][0] -= other.data_[2][0];
    data_[2][1] -= other.data_[2][1];
    data_[2][2] -= other.data_[2][2];
    data_[2][3] -= other.data_[2][3];

    data_[3][0] -= other.data_[3][0];
    data_[3][1] -= other.data_[3][1];
    data_[3][2] -= other.data_[3][2];
    data_[3][3] -= other.data_[3][3];
    return *this;
}

template<typename Type>
core_inline_
mat4<Type> &mat4<Type>::operator*=(const mat4<Type> &other)
{
    data_[0][0] *= other.data_[0][0];
    data_[0][1] *= other.data_[0][1];
    data_[0][2] *= other.data_[0][2];
    data_[0][3] *= other.data_[0][3];

    data_[1][0] *= other.data_[1][0];
    data_[1][1] *= other.data_[1][1];
    data_[1][2] *= other.data_[1][2];
    data_[1][3] *= other.data_[1][3];

    data_[2][0] *= other.data_[2][0];
    data_[2][1] *= other.data_[2][1];
    data_[2][2] *= other.data_[2][2];
    data_[2][3] *= other.data_[2][3];

    data_[3][0] *= other.data_[3][0];
    data_[3][1] *= other.data_[3][1];
    data_[3][2] *= other.data_[3][2];
    data_[3][3] *= other.data_[3][3];
    return *this;
}

template<typename Type>
core_inline_
mat4<Type> &mat4<Type>::operator/=(const mat4<Type> &other)
{
    data_[0][0] /= other.data_[0][0];
    data_[0][1] /= other.data_[0][1];
    data_[0][2] /= other.data_[0][2];
    data_[0][3] /= other.data_[0][3];

    data_[1][0] /= other.data_[1][0];
    data_[1][1] /= other.data_[1][1];
    data_[1][2] /= other.data_[1][2];
    data_[1][3] /= other.data_[1][3];

    data_[2][0] /= other.data_[2][0];
    data_[2][1] /= other.data_[2][1];
    data_[2][2] /= other.data_[2][2];
    data_[2][3] /= other.data_[2][3];

    data_[3][0] /= other.data_[3][0];
    data_[3][1] /= other.data_[3][1];
    data_[3][2] /= other.data_[3][2];
    data_[3][3] /= other.data_[3][3];
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<typename Type>
core_inline_
mat4<Type> &mat4<Type>::operator+=(const Type scalar)
{
    data_[0][0] += scalar;
    data_[0][1] += scalar;
    data_[0][2] += scalar;
    data_[0][3] += scalar;

    data_[1][0] += scalar;
    data_[1][1] += scalar;
    data_[1][2] += scalar;
    data_[1][3] += scalar;

    data_[2][0] += scalar;
    data_[2][1] += scalar;
    data_[2][2] += scalar;
    data_[2][3] += scalar;

    data_[3][0] += scalar;
    data_[3][1] += scalar;
    data_[3][2] += scalar;
    data_[3][3] += scalar;
    return *this;
}

template<typename Type>
core_inline_
mat4<Type> &mat4<Type>::operator-=(const Type scalar)
{
    data_[0][0] -= scalar;
    data_[0][1] -= scalar;
    data_[0][2] -= scalar;
    data_[0][3] -= scalar;

    data_[1][0] -= scalar;
    data_[1][1] -= scalar;
    data_[1][2] -= scalar;
    data_[1][3] -= scalar;

    data_[2][0] -= scalar;
    data_[2][1] -= scalar;
    data_[2][2] -= scalar;
    data_[2][3] -= scalar;

    data_[3][0] -= scalar;
    data_[3][1] -= scalar;
    data_[3][2] -= scalar;
    data_[3][3] -= scalar;
    return *this;
}

template<typename Type>
core_inline_
mat4<Type> &mat4<Type>::operator*=(const Type scalar)
{
    data_[0][0] *= scalar;
    data_[0][1] *= scalar;
    data_[0][2] *= scalar;
    data_[0][3] *= scalar;

    data_[1][0] *= scalar;
    data_[1][1] *= scalar;
    data_[1][2] *= scalar;
    data_[1][3] *= scalar;

    data_[2][0] *= scalar;
    data_[2][1] *= scalar;
    data_[2][2] *= scalar;
    data_[2][3] *= scalar;

    data_[3][0] *= scalar;
    data_[3][1] *= scalar;
    data_[3][2] *= scalar;
    data_[3][3] *= scalar;
    return *this;
}

template<typename Type>
core_inline_
mat4<Type> &mat4<Type>::operator/=(const Type scalar)
{
    data_[0][0] /= scalar;
    data_[0][1] /= scalar;
    data_[0][2] /= scalar;
    data_[0][3] /= scalar;

    data_[1][0] /= scalar;
    data_[1][1] /= scalar;
    data_[1][2] /= scalar;
    data_[1][3] /= scalar;

    data_[2][0] /= scalar;
    data_[2][1] /= scalar;
    data_[2][2] /= scalar;
    data_[2][3] /= scalar;

    data_[3][0] /= scalar;
    data_[3][1] /= scalar;
    data_[3][2] /= scalar;
    data_[3][3] /= scalar;
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Increment operators.
 */
template<typename Type>
core_inline_
mat4<Type> &mat4<Type>::operator++ (void)
{
    constexpr Type one = (Type) 1;
    (*this) += one;
    return *this;
}

template<typename Type>
core_inline_
mat4<Type> &mat4<Type>::operator-- (void)
{
    constexpr Type one = (Type) 1;
    (*this) -= one;
    return *this;
}


template<typename Type>
core_inline_
mat4<Type> mat4<Type>::operator++ (int)
{
    mat4<Type> result(*this);
    ++(*this);
    return result;
}

template<typename Type>
core_inline_
mat4<Type> mat4<Type>::operator-- (int)
{
    mat4<Type> result(*this);
    --(*this);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between two vectors.
 */
template<typename Type>
core_inline_
mat4<Type> operator+ (mat4<Type> lhs, const mat4<Type> &rhs)
{
    lhs += rhs;
    return lhs;
}

template<typename Type>
core_inline_
mat4<Type> operator- (mat4<Type> lhs, const mat4<Type> &rhs)
{
    lhs -= rhs;
    return lhs;
}

template<typename Type>
core_inline_
mat4<Type> operator* (mat4<Type> lhs, const mat4<Type> &rhs)
{
    lhs *= rhs;
    return lhs;
}

template<typename Type>
core_inline_
mat4<Type> operator/ (mat4<Type> lhs, const mat4<Type> &rhs)
{
    lhs /= rhs;
    return lhs;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
template<typename Type>
core_inline_
mat4<Type> operator+ (mat4<Type> lhs, const Type scalar)
{
    lhs += scalar;
    return lhs;
}

template<typename Type>
core_inline_
mat4<Type> operator- (mat4<Type> lhs, const Type scalar)
{
    lhs -= scalar;
    return lhs;
}

template<typename Type>
core_inline_
mat4<Type> operator* (mat4<Type> lhs, const Type scalar)
{
    lhs *= scalar;
    return lhs;
}

template<typename Type>
core_inline_
mat4<Type> operator/ (mat4<Type> lhs, const Type scalar)
{
    lhs /= scalar;
    return lhs;
}

} /* namespace math */
} /* namespace core */


/** ---------------------------------------------------------------------------
 * @brief Enable simd vectorized instructions.
 */
#ifdef CORE_USE_SIMD
#include "mat4-simd.hpp"
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

#endif /* CORE_GEOMETRY_MAT4_H_ */
