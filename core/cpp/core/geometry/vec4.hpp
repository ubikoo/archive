/*
 * vec4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_VEC4_H_
#define CORE_GEOMETRY_VEC4_H_

namespace core {
namespace math {

/** ---------------------------------------------------------------------------
 * struct vec4
 * @brief A templated 4-dimensional vector type.
 */
template<typename Type>
struct vec4 {
    /* Types */
    typedef std::size_t     size_type;
    typedef Type            value_type;
    typedef Type&           reference;
    typedef const Type&     const_reference;

    /* Static constants */
    static const size_type length_ = 4;

    /* Member variables/accessors */
    value_type data_[length_] core_aligned_;

    /* Capacity */
    size_type length(void) const { return length_; }
    size_type size(void) const { return sizeof(data_); }

    /* Element access */
    value_type *data(void) { return data_; }
    const value_type *data(void) const { return data_; }

    reference operator()(size_type i) { return data_[i]; }
    const_reference operator()(size_type i) const { return data_[i]; }

    /* Unary arithmetic vector operators. */
    vec4 &operator+=(const vec4 &other);
    vec4 &operator-=(const vec4 &other);
    vec4 &operator*=(const vec4 &other);
    vec4 &operator/=(const vec4 &other);

    /* Unary arithmetic scalar operators. */
    vec4 &operator+=(const Type scalar);
    vec4 &operator-=(const Type scalar);
    vec4 &operator*=(const Type scalar);
    vec4 &operator/=(const Type scalar);

    /* Increment operators. */
    vec4 &operator++ (void);
    vec4 &operator-- (void);
    vec4 operator++ (int);
    vec4 operator-- (int);

    /* Constructor/destructor. */
    vec4 () { std::memset(data_, 0, sizeof(data_)); }
    explicit vec4 (const value_type *data) {
        std::memcpy(data_, data, sizeof(data_));
    }
    explicit vec4 (const Type e0) {
        data_[0] = e0;
        data_[1] = e0;
        data_[2] = e0;
        data_[3] = e0;
    }
    explicit vec4 (const Type e0, const Type e1, const Type e2, const Type e3) {
        data_[0] = e0;
        data_[1] = e1;
        data_[2] = e2;
        data_[3] = e3;
    }
    ~vec4 () = default;

    /* Copy constructor/assignment. */
    vec4 (const vec4 &other) {
        std::memcpy(data_, other.data_, sizeof(data_));
    }
    vec4 &operator=(const vec4 &other) {
        if (this == &other) {
            return *this;
        }
        std::memcpy(data_, other.data_, sizeof(data_));
        return *this;
    }
}; /* struct vec4 */


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
template<typename Type>
core_inline_
vec4<Type> &vec4<Type>::operator+=(const vec4<Type> &other)
{
    data_[0] += other.data_[0];
    data_[1] += other.data_[1];
    data_[2] += other.data_[2];
    data_[3] += other.data_[3];
    return *this;
}

template<typename Type>
core_inline_
vec4<Type> &vec4<Type>::operator-=(const vec4<Type> &other)
{
    data_[0] -= other.data_[0];
    data_[1] -= other.data_[1];
    data_[2] -= other.data_[2];
    data_[3] -= other.data_[3];
    return *this;
}

template<typename Type>
core_inline_
vec4<Type> &vec4<Type>::operator*=(const vec4<Type> &other)
{
    data_[0] *= other.data_[0];
    data_[1] *= other.data_[1];
    data_[2] *= other.data_[2];
    data_[3] *= other.data_[3];
    return *this;
}

template<typename Type>
core_inline_
vec4<Type> &vec4<Type>::operator/=(const vec4<Type> &other)
{
    data_[0] /= other.data_[0];
    data_[1] /= other.data_[1];
    data_[2] /= other.data_[2];
    data_[3] /= other.data_[3];
    return *this;
}

/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<typename Type>
core_inline_
vec4<Type> &vec4<Type>::operator+=(const Type scalar)
{
    data_[0] += scalar;
    data_[1] += scalar;
    data_[2] += scalar;
    data_[3] += scalar;
    return *this;
}

template<typename Type>
core_inline_
vec4<Type> &vec4<Type>::operator-=(const Type scalar)
{
    data_[0] -= scalar;
    data_[1] -= scalar;
    data_[2] -= scalar;
    data_[3] -= scalar;
    return *this;
}

template<typename Type>
core_inline_
vec4<Type> &vec4<Type>::operator*=(const Type scalar)
{
    data_[0] *= scalar;
    data_[1] *= scalar;
    data_[2] *= scalar;
    data_[3] *= scalar;
    return *this;
}

template<typename Type>
core_inline_
vec4<Type> &vec4<Type>::operator/=(const Type scalar)
{
    data_[0] /= scalar;
    data_[1] /= scalar;
    data_[2] /= scalar;
    data_[3] /= scalar;
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Increment operators.
 */
template<typename Type>
core_inline_
vec4<Type> &vec4<Type>::operator++ (void)
{
    constexpr Type one = (Type) 1;
    (*this) += one;
    return *this;
}

template<typename Type>
core_inline_
vec4<Type> &vec4<Type>::operator-- (void)
{
    constexpr Type one = (Type) 1;
    (*this) -= one;
    return *this;
}


template<typename Type>
core_inline_
vec4<Type> vec4<Type>::operator++ (int)
{
    vec4<Type> result(*this);
    ++(*this);
    return result;
}

template<typename Type>
core_inline_
vec4<Type> vec4<Type>::operator-- (int)
{
    vec4<Type> result(*this);
    --(*this);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between two vectors.
 */
template<typename Type>
core_inline_
vec4<Type> operator+ (vec4<Type> lhs, const vec4<Type> &rhs)
{
    lhs += rhs;
    return lhs;
}

template<typename Type>
core_inline_
vec4<Type> operator- (vec4<Type> lhs, const vec4<Type> &rhs)
{
    lhs -= rhs;
    return lhs;
}

template<typename Type>
core_inline_
vec4<Type> operator* (vec4<Type> lhs, const vec4<Type> &rhs)
{
    lhs *= rhs;
    return lhs;
}

template<typename Type>
core_inline_
vec4<Type> operator/ (vec4<Type> lhs, const vec4<Type> &rhs)
{
    lhs /= rhs;
    return lhs;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
template<typename Type>
core_inline_
vec4<Type> operator+ (vec4<Type> lhs, const Type scalar)
{
    lhs += scalar;
    return lhs;
}

template<typename Type>
core_inline_
vec4<Type> operator- (vec4<Type> lhs, const Type scalar)
{
    lhs -= scalar;
    return lhs;
}

template<typename Type>
core_inline_
vec4<Type> operator* (vec4<Type> lhs, const Type scalar)
{
    lhs *= scalar;
    return lhs;
}

template<typename Type>
core_inline_
vec4<Type> operator/ (vec4<Type> lhs, const Type scalar)
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
#include "vec4-simd.hpp"
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

#endif /* CORE_GEOMETRY_VEC4_H_ */