/*
 * linalg-arithmetic.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_LINALG_ARITHMETIC_H_
#define ALGO_NUMERIC_LINALG_ARITHMETIC_H_

#include "atto/core/core.hpp"
#include "numeric/array/array.hpp"

namespace algo {

/** ---- Matrix-Vector multiplication -----------------------------------------
 * matmul_vector<T>
 * @brief Multiply matrix A by vector B, A * B = C,
 *
 * C(i) = A(i,k)*B(k),
 *
 * where C(i) is the dot product of the row-i of matrix A and vector B.
 *
 * m is the number of rows in matrix A and in vector C
 * n is the number of columns in matrix A and rows in vector B
 * A is a pointer to matrix A
 * B is a pointer to vector B
 * C is a pointer to vector C
 *
 * T is assumed to be a primitive data type.
 */
template<typename T>
core_inline
void matmul_vector_row(
    size_t i,
    Matrix<T> & core_restrict(A),
    Vector<T> & core_restrict(B),
    Vector<T> & core_restrict(C))
{
    T sum = 0.0;
    for (size_t j = 0; j < A.n2(); ++j) {
        sum += A(i,j) * B(j);
    }
    C(i) = sum;
}

template<typename T, bool Is_par = true>
void matmul_vector(
    Matrix<T> & core_restrict(A),
    Vector<T> & core_restrict(B),
    Vector<T> & core_restrict(C))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(C.n1() == A.n1() && B.n1() == A.n2(), "invalid dimensions");

    /* C(i) = A(i,j) * B(j) */
    core_pragma_omp(parallel for if(Is_par) default(none) \
        shared(A, B, C) schedule(static))
    for (size_t i = 0; i < C.n1(); ++i) {
        matmul_vector_row<T>(i, A, B, C);
    }
}


/** ---- Matrix-Matrix multiplication -----------------------------------------
 * matmul_matrix<T>
 * @brief Multiply matrix A by matrix B, A * B = C,
 *
 * C(i,j) = A(i,k)*B(k,j),
 *
 * where C(i,j) is the dot product of the row i of matrix A and the vector
 * column j of matrix B.
 *
 * m is the number of rows in matrix A and in matrix C
 * n is the number of columns in matrix A and rows in matrix B
 * p is the number of columns in matrix B and in matrix C
 * A is a pointer to matrix A
 * B is a pointer to matrix B
 * C is a pointer to result C
 */
template<typename T>
core_inline
void matmul_matrix_row(
    size_t i,
    Matrix<T> & core_restrict(A),
    Matrix<T> & core_restrict(B),
    Matrix<T> & core_restrict(C))
{
    for (size_t j = 0; j < C.n2(); ++j) {
        T sum = 0.0;
        for (size_t k = 0; k < A.n2(); ++k) {
            sum += A(i,k) * B(k,j);
        }
        C(i,j) = sum;
    }
}


template<typename T, bool Is_par = true>
void matmul_matrix(
    Matrix<T> & core_restrict(A),
    Matrix<T> & core_restrict(B),
    Matrix<T> & core_restrict(C))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(C.n1() == A.n1() &&
                C.n2() == B.n2() &&
                A.n2() == B.n1(),
                "invalid dimensions");

    /* C(i,j) = A(i,k) * B(k,j) */
    core_pragma_omp(parallel for if(Is_par) default(none) \
        shared(A, B, C) schedule(static))
    for (size_t i = 0; i < C.n1(); ++i) {
        matmul_matrix_row<T>(i, A, B, C);
    }
}


/** ---- Zero-vector operator -------------------------------------------------
 * zero_vector<T>
 * @brief Zero the vector elements.
 */
template<typename T, bool IsPar = true>
void zero_vector(Vector<T> & core_restrict(vec))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(vec.n1() > 0, "invalid dimensions");

    constexpr T zero = (T) 0;
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(vec) schedule(static))
    for (size_t i = 0; i < vec.n1(); ++i) {
        vec(i) = zero;
    }
}


/** ---- Zero-matrix operator -------------------------------------------------
 * zero_matrix<T>
 * @brief Zero the matrix elements.
 */
template<typename T, bool IsPar = true>
void zero_matrix(Matrix<T> & core_restrict(mat))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(mat.n1() > 0 && mat.n2() > 0, "invalid dimensions");

    constexpr T zero = (T) 0;
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(mat) schedule(static))
    for (size_t i = 0; i < mat.n1(); ++i) {
        for (size_t j = 0; j < mat.n2(); ++j) {
            mat(i,j) = zero;
        }
    }
}


/** ---- Zero-tensor operator -------------------------------------------------
 * zero_tensor<T>
 * @brief Zero the tensor elements.
 */
template<typename T, bool IsPar = true>
void zero_tensor(Tensor<T> & core_restrict(tns))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(tns.n1() > 0 &&
                tns.n2() > 0 &&
                tns.n3() > 0,
                "invalid dimensions");

    constexpr T zero = (T) 0;
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(tns) schedule(static))
    for (size_t i = 0; i < tns.n1(); ++i) {
        for (size_t j = 0; j < tns.n2(); ++j) {
            for (size_t k = 0; k < tns.n3(); ++k) {
                tns(i,j,k) = zero;
            }
        }
    }
}


/** ---- Copy-vector operator -------------------------------------------------
 * copy_vector<T>
 * @brief Copy the vector elements.
 */
template<typename T, bool IsPar = true>
void copy_vector(
    Vector<T> & core_restrict(src),
    Vector<T> & core_restrict(dst))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    core_assert(src.n1() > 0  &&
                src.n1() == dst.n1(),
                "invalid dimensions");

    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(src, dst) schedule(static))
    for (size_t i = 0; i < src.n1(); ++i) {
        dst(i) = src(i);
    }
}


/** ---- Copy-matrix operator -------------------------------------------------
 * copy_matrix<T>
 * @brief Copy the matrix elements.
 */
template<typename T, bool IsPar = true>
void copy_matrix(
    Matrix<T> & core_restrict(src),
    Matrix<T> & core_restrict(dst))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(src.n1() > 0           &&
                src.n2() > 0           &&
                src.n1() == dst.n1()   &&
                src.n2() == dst.n2(),
                "invalid dimensions");

    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(src, dst) schedule(static))
    for (size_t i = 0; i < src.n1(); ++i) {
        for (size_t j = 0; j < src.n2(); ++j) {
            dst(i,j) = src(i,j);
        }
    }
}


/** ---- Copy-tensor operator -------------------------------------------------
 * copy_tensor<T>
 * @brief Copy the tensor elements.
 */
template<typename T, bool IsPar = true>
void copy_tensor(
    Tensor<T> & core_restrict(src),
    Tensor<T> & core_restrict(dst))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(src.n1() > 0           &&
                src.n2() > 0           &&
                src.n3() > 0           &&
                src.n1() == dst.n1()   &&
                src.n2() == dst.n2()   &&
                src.n3() == dst.n3(),
                "invalid dimensions");

    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(src, dst) schedule(static))
    for (size_t i = 0; i < src.n1(); ++i) {
        for (size_t j = 0; j < src.n2(); ++j) {
            for (size_t k = 0; k < src.n3(); ++k) {
                dst(i,j,k) = src(i,j,k);
            }
        }
    }
}


/** ---- Diagonal-matrix operator ---------------------------------------------
 * diag_matrix<T>
 * @brief Compute diagonal matrix from vector elements.
 */
template<typename T, bool IsPar = true>
void diag_matrix(
    Vector<T> & core_restrict(vec),
    Matrix<T> & core_restrict(mat))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(vec.n1() > 0           &&
                mat.n1() > 0           &&
                mat.n2() > 0           &&
                vec.n1() == mat.n1()   &&
                vec.n1() == mat.n2(),
                "invalid dimensions");

    /* Zero the matrix  and compute the diagonal */
    zero_matrix<T,IsPar>(mat);
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(mat, vec) schedule(static))
    for (size_t i = 0; i < mat.n1(); ++i) {
        mat(i,i) = vec(i);
    }
}


/** ---- Diagonal-vector operator ---------------------------------------------
 * diag_vector<T>
 * @brief Compute vector from matrix diagonal.
 */
template<typename T, bool IsPar = true>
void diag_vector(Matrix<T> & core_restrict(mat),
                  Vector<T> & core_restrict(vec))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(vec.n1() > 0           &&
                mat.n1() > 0           &&
                mat.n2() > 0           &&
                vec.n1() == mat.n1()   &&
                vec.n1() == mat.n2(),
                "invalid dimensions");

    /* Zero the vector and extract the diagonal. */
    zero_vector<T,IsPar>(vec);

    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(mat, vec) schedule(static))
    for (size_t i = 0; i < vec.n1(); ++i) {
        vec(i) = mat(i,i);
    }
}


/** ---- Transpose-matrix operator --------------------------------------------
 * transpose_matrix<T>
 * @brief Transpose matrix.
 */
template<typename T, bool IsPar = true>
void transpose_matrix(
    Matrix<T> & core_restrict(mat),
    Matrix<T> & core_restrict(mat_tr))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(mat.n1() > 0            &&
                mat.n2() > 0            &&
                mat_tr.n1() == mat.n2() &&
                mat_tr.n2() == mat.n1(),
                "invalid dimensions");

    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(mat, mat_tr) schedule (dynamic))
    for (size_t i = 0; i < mat.n1(); ++i) {
        for (size_t j = i; j < mat.n2(); ++j) {
            mat_tr(i,j) = mat(j,i);
            mat_tr(j,i) = mat(i,j);
        }
    }
}


/** ---- Identity-matrix operator ---------------------------------------------
 * identity_matrix<T>
 * @brief Identity matrix.
 */
template<typename T, bool IsPar = true>
void identity_matrix(Matrix<T> & core_restrict(mat))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    core_assert(mat.n1() > 0           &&
                mat.n2() > 0           &&
                mat.n2() == mat.n1(),
                "invalid dimensions");

    /* Zero the matrix */
    zero_matrix<T,IsPar>(mat);

    constexpr T one = (T) 1;
    core_pragma_omp(parallel for if(IsPar) default(none) \
        shared(mat) schedule(static))
    for (size_t i = 0; i < mat.n1(); ++i) {
        mat(i,i) = one;
    }
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_LINALG_ARITHMETIC_H_ */
