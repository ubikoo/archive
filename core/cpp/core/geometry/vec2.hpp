/*
 * vec2.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_GEOMETRY_VEC2_H_
#define CORE_GEOMETRY_VEC2_H_

namespace core {
namespace math {

/** ---------------------------------------------------------------------------
 * struct vec2
 * @brief A templated 2-dimensional vector type.
 */
template<typename Type>
struct vec2 {
    /* Types */
    typedef std::size_t     size_type;
    typedef Type            value_type;
    typedef Type&           reference;
    typedef const Type&     const_reference;

    /* Static constants */
    static const size_type length_ = 2;

    /* Member variables/accessors */
    value_type data_[length_] core_aligned_;

    /* Capacity */
    size_type length(void) const { return length_; }
    size_type size(void) const { return sizeof(data_); }

    /* Element access */
    value_type *data(void) { return &data_[0]; }
    const value_type *data(void) const { return &data_[0]; }

    reference operator()(size_type i) { return data_[i]; }
    const_reference operator()(size_type i) const { return data_[i]; }

    /* Unary arithmetic vector operators. */
    vec2 &operator+=(const vec2 &other);
    vec2 &operator-=(const vec2 &other);
    vec2 &operator*=(const vec2 &other);
    vec2 &operator/=(const vec2 &other);

    /* Unary arithmetic scalar operators. */
    vec2 &operator+=(const Type scalar);
    vec2 &operator-=(const Type scalar);
    vec2 &operator*=(const Type scalar);
    vec2 &operator/=(const Type scalar);

    /* Increment operators. */
    vec2 &operator++ (void);
    vec2 &operator-- (void);
    vec2 operator++ (int);
    vec2 operator-- (int);

    /* Constructor/destructor. */
    vec2 () { std::memset(data_, 0, sizeof(data_)); }
    explicit vec2 (const value_type *data) {
        std::memcpy(data_, data, sizeof(data_));
    }
    explicit vec2 (const Type e0) {
        data_[0] = e0;
        data_[1] = e0;
    }
    explicit vec2 (const Type e0, const Type e1) {
        data_[0] = e0;
        data_[1] = e1;
    }
    ~vec2 () = default;

    /* Copy constructor/assignment. */
    vec2 (const vec2 &other) {
        std::memcpy(data_, other.data_, sizeof(data_));
    }
    vec2 &operator=(const vec2 &other) {
        if (this == &other) {
            return *this;
        }
        std::memcpy(data_, other.data_, sizeof(data_));
        return *this;
    }
}; /* struct vec2 */


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic vector operators.
 */
template<typename Type>
core_inline_
vec2<Type> &vec2<Type>::operator+=(const vec2<Type> &other)
{
    data_[0] += other.data_[0];
    data_[1] += other.data_[1];
    return *this;
}

template<typename Type>
core_inline_
vec2<Type> &vec2<Type>::operator-=(const vec2<Type> &other)
{
    data_[0] -= other.data_[0];
    data_[1] -= other.data_[1];
    return *this;
}

template<typename Type>
core_inline_
vec2<Type> &vec2<Type>::operator*=(const vec2<Type> &other)
{
    data_[0] *= other.data_[0];
    data_[1] *= other.data_[1];
    return *this;
}

template<typename Type>
core_inline_
vec2<Type> &vec2<Type>::operator/=(const vec2<Type> &other)
{
    data_[0] /= other.data_[0];
    data_[1] /= other.data_[1];
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Unary arithmetic scalar operators.
 */
template<typename Type>
core_inline_
vec2<Type> &vec2<Type>::operator+=(const Type scalar)
{
    data_[0] += scalar;
    data_[1] += scalar;
    return *this;
}

template<typename Type>
core_inline_
vec2<Type> &vec2<Type>::operator-=(const Type scalar)
{
    data_[0] -= scalar;
    data_[1] -= scalar;
    return *this;
}

template<typename Type>
core_inline_
vec2<Type> &vec2<Type>::operator*=(const Type scalar)
{
    data_[0] *= scalar;
    data_[1] *= scalar;
    return *this;
}

template<typename Type>
core_inline_
vec2<Type> &vec2<Type>::operator/=(const Type scalar)
{
    data_[0] /= scalar;
    data_[1] /= scalar;
    return *this;
}


/** ---------------------------------------------------------------------------
 * @brief Increment operators.
 */
template<typename Type>
core_inline_
vec2<Type> &vec2<Type>::operator++ (void)
{
    constexpr Type one = (Type) 1;
    (*this) += one;
    return *this;
}

template<typename Type>
core_inline_
vec2<Type> &vec2<Type>::operator-- (void)
{
    constexpr Type one = (Type) 1;
    (*this) -= one;
    return *this;
}


template<typename Type>
core_inline_
vec2<Type> vec2<Type>::operator++ (int)
{
    vec2<Type> result(*this);
    ++(*this);
    return result;
}

template<typename Type>
core_inline_
vec2<Type> vec2<Type>::operator-- (int)
{
    vec2<Type> result(*this);
    --(*this);
    return result;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between two vectors.
 */
template<typename Type>
core_inline_
vec2<Type> operator+ (vec2<Type> lhs, const vec2<Type> &rhs)
{
    lhs += rhs;
    return lhs;
}

template<typename Type>
core_inline_
vec2<Type> operator- (vec2<Type> lhs, const vec2<Type> &rhs)
{
    lhs -= rhs;
    return lhs;
}

template<typename Type>
core_inline_
vec2<Type> operator* (vec2<Type> lhs, const vec2<Type> &rhs)
{
    lhs *= rhs;
    return lhs;
}

template<typename Type>
core_inline_
vec2<Type> operator/ (vec2<Type> lhs, const vec2<Type> &rhs)
{
    lhs /= rhs;
    return lhs;
}


/** ---------------------------------------------------------------------------
 * @brief Binary arithmetic operators between a vector and a scalar.
 */
template<typename Type>
core_inline_
vec2<Type> operator+ (vec2<Type> lhs, const Type scalar)
{
    lhs += scalar;
    return lhs;
}

template<typename Type>
core_inline_
vec2<Type> operator- (vec2<Type> lhs, const Type scalar)
{
    lhs -= scalar;
    return lhs;
}

template<typename Type>
core_inline_
vec2<Type> operator* (vec2<Type> lhs, const Type scalar)
{
    lhs *= scalar;
    return lhs;
}

template<typename Type>
core_inline_
vec2<Type> operator/ (vec2<Type> lhs, const Type scalar)
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
#include "vec2-simd.hpp"
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

#endif /* CORE_GEOMETRY_VEC2_H_ */