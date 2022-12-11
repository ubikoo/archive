/*
 * mat2.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_MAT2_H_
#define CORE_GEOMETRY_MAT2_H_

namespace core {
namespace math {

/** ---- mat2 class -----------------------------------------------------------
 *
 * struct mat2
 * @brief
 * mat2 is a plain class representing a 2x2-component matrix of type double.
 */
template<typename Type>
struct mat2 {
    /* Types */
    typedef std::size_t     size_type;
    typedef Type            value_type;
    typedef Type&           reference;
    typedef const Type&     const_reference;

    /* Static constants */
    static const size_type dim_ = 2;
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
    mat2 &operator+=(const mat2 &other);
    mat2 &operator-=(const mat2 &other);
    mat2 &operator*=(const mat2 &other);
    mat2 &operator/=(const mat2 &other);

    /* Unary arithmetic scalar operators. */
    mat2 &operator+=(const Type scalar);
    mat2 &operator-=(const Type scalar);
    mat2 &operator*=(const Type scalar);
    mat2 &operator/=(const Type scalar);

    /* Increment operators. */
    mat2 &operator++ (void);
    mat2 &operator-- (void);
    mat2 operator++ (int);
    mat2 operator-- (int);

    /* Constructor/destructor. */
    mat2 () { std::memset(data_, 0, sizeof(data_)); }
    explicit mat2 (const value_type *data) {
        std::memcpy(data_, data, sizeof(data_));
    }
    explicit mat2 (const Type e0) {
        data_[0][0] = e0;
        data_[0][1] = e0;

        data_[1][0] = e0;
        data_[1][1] = e0;
    }
    explicit mat2 (
        const Type e0,
        const Type e1,
        const Type e2,
        const Type e3) {
        data_[0][0] = e0;
        data_[0][1] = e1;

        data_[1][0] = e2;
        data_[1][1] = e3;
    }
    ~mat2 () = default;

    /* Copy constructor/assignment. */
    mat2 (const mat2 &other) {
        std::memcpy(data_, other.data_, sizeof(data_));
    }
    mat2 &operator=(const mat2 &other) {
        if (this == &other) {
            return *this;
        }
        std::memcpy(data_, other.data_, sizeof(data_));
        return *this;
    }
}; /* struct mat2 */


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
template<typename Type>
core_inline_
mat2<Type> &mat2<Type>::operator+=(const mat2<Type> &other)
{
    data_[0][0] += other.data_[0][0];
    data_[0][1] += other.data_[0][1];

    data_[1][0] += other.data_[1][0];
    data_[1][1] += other.data_[1][1];
    return *this;
}

template<typename Type>
core_inline_
mat2<Type> &mat2<Type>::operator-=(const mat2<Type> &other)
{
    data_[0][0] -= other.data_[0][0];
    data_[0][1] -= other.data_[0][1];

    data_[1][0] -= other.data_[1][0];
    data_[1][1] -= other.data_[1][1];
    return *this;
}

template<typename Type>
core_inline_
mat2<Type> &mat2<Type>::operator*=(const mat2<Type> &other)
{
    data_[0][0] *= other.data_[0][0];
    data_[0][1] *= other.data_[0][1];

    data_[1][0] *= other.data_[1][0];
    data_[1][1] *= other.data_[1][1];
    return *this;
}

template<typename Type>
core_inline_
mat2<Type> &mat2<Type>::operator/=(const mat2<Type> &other)
{
    data_[0][0] /= other.data_[0][0];
    data_[0][1] /= other.data_[0][1];

    data_[1][0] /= other.data_[1][0];
    data_[1][1] /= other.data_[1][1];
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<typename Type>
core_inline_
mat2<Type> &mat2<Type>::operator+=(const Type scalar)
{
    data_[0][0] += scalar;
    data_[0][1] += scalar;

    data_[1][0] += scalar;
    data_[1][1] += scalar;
    return *this;
}

template<typename Type>
core_inline_
mat2<Type> &mat2<Type>::operator-=(const Type scalar)
{
    data_[0][0] -= scalar;
    data_[0][1] -= scalar;

    data_[1][0] -= scalar;
    data_[1][1] -= scalar;
    return *this;
}

template<typename Type>
core_inline_
mat2<Type> &mat2<Type>::operator*=(const Type scalar)
{
    data_[0][0] *= scalar;
    data_[0][1] *= scalar;

    data_[1][0] *= scalar;
    data_[1][1] *= scalar;
    return *this;
}

template<typename Type>
core_inline_
mat2<Type> &mat2<Type>::operator/=(const Type scalar)
{
    data_[0][0] /= scalar;
    data_[0][1] /= scalar;

    data_[1][0] /= scalar;
    data_[1][1] /= scalar;
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Increment operators.
 */
template<typename Type>
core_inline_
mat2<Type> &mat2<Type>::operator++ (void)
{
    constexpr Type one = (Type) 1;
    (*this) += one;
    return *this;
}

template<typename Type>
core_inline_
mat2<Type> &mat2<Type>::operator-- (void)
{
    constexpr Type one = (Type) 1;
    (*this) -= one;
    return *this;
}


template<typename Type>
core_inline_
mat2<Type> mat2<Type>::operator++ (int)
{
    mat2<Type> result(*this);
    ++(*this);
    return result;
}

template<typename Type>
core_inline_
mat2<Type> mat2<Type>::operator-- (int)
{
    mat2<Type> result(*this);
    --(*this);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between two vectors.
 */
template<typename Type>
core_inline_
mat2<Type> operator+ (mat2<Type> lhs, const mat2<Type> &rhs)
{
    lhs += rhs;
    return lhs;
}

template<typename Type>
core_inline_
mat2<Type> operator- (mat2<Type> lhs, const mat2<Type> &rhs)
{
    lhs -= rhs;
    return lhs;
}

template<typename Type>
core_inline_
mat2<Type> operator* (mat2<Type> lhs, const mat2<Type> &rhs)
{
    lhs *= rhs;
    return lhs;
}

template<typename Type>
core_inline_
mat2<Type> operator/ (mat2<Type> lhs, const mat2<Type> &rhs)
{
    lhs /= rhs;
    return lhs;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
template<typename Type>
core_inline_
mat2<Type> operator+ (mat2<Type> lhs, const Type scalar)
{
    lhs += scalar;
    return lhs;
}

template<typename Type>
core_inline_
mat2<Type> operator- (mat2<Type> lhs, const Type scalar)
{
    lhs -= scalar;
    return lhs;
}

template<typename Type>
core_inline_
mat2<Type> operator* (mat2<Type> lhs, const Type scalar)
{
    lhs *= scalar;
    return lhs;
}

template<typename Type>
core_inline_
mat2<Type> operator/ (mat2<Type> lhs, const Type scalar)
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
#include "mat2-simd.hpp"
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

#endif /* CORE_GEOMETRY_MAT2_H_ */
