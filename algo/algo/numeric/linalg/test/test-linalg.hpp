/*
 * test_linalg.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ALGO_NUMERIC_LINALG_H_
#define TEST_ALGO_NUMERIC_LINALG_H_

#include "test/common.hpp"
#include "atto/core/core.hpp"
#include "atto/math/math.hpp"
#include "numeric/linalg/linalg.hpp"

/** ---- Test Gauss Solver ----------------------------------------------------
 * test_linalg_gauss<T>
 */
template<typename T>
void test_linalg_gauss(const size_t ndim)
{
    /*
     * Create random number generator,
     */
    atto::math::rng::Kiss kiss(true);
    atto::math::rng::gauss<T> nrand;
    atto::math::rng::uniform<T> urand;

    /*
     * Create a random matrix A, random vector B and solution vector X.
     */
    algo::Matrix<T> A(ndim, ndim);
    algo::Vector<T> B(ndim);
    algo::Vector<T> X(ndim);
    for (size_t i = 0; i < ndim; ++i) {
        /* Sample the random matrix. */
        for (size_t j = 0; j < ndim; ++j) {
            A(i,j) = urand(kiss);
        }

        /* Condition the main diagonal with a positive random number. */
        A(i,i) += 1.0 + urand(kiss);

        /* Sample the random vector. */
        B(i) = urand(kiss);
    }

    /*
     * Solve the linear problem.
     */
    algo::Matrix<T> AA(ndim, ndim);
    algo::Vector<T> BB(ndim);
    for (size_t i = 0; i < ndim; ++i) {
        for (size_t j = 0; j < ndim; ++j) {
            AA(i,j) = A(i,j);
        }
        BB(i) = B(i);
    }

    if (ndim < 4000) {
        algo::gauss_solve<T>(AA, BB, X);
    } else {
        algo::gauss_solve_omp<T>(AA, BB, X);
    }

    /*
     * Compute the error.
     */
    algo::Vector<T> C(ndim);
    algo::matmul_vector<T>(A, X, C);

    double err = 0.0;
    for (size_t i = 0; i < ndim; ++i) {
        err += static_cast<double>(atto::math::fabs<T>(C(i) - B(i)));
    }
    err /= (double) ndim;
    std::printf("%s: error %.15e\n", __FUNCTION__, err);

    if (typeid(T) == typeid(double)) {
        REQUIRE(err < 1.0E-8);
    } else if (typeid(T) == typeid(float)) {
        REQUIRE(err < 1.0);
    }

    ///* Print the matrix */
    //for (size_t i = 0; i < ndim; ++i) {
        //for (size_t j = 0; j < ndim; ++j) {
            //std::printf(" %g", A(i,j));
        //}
        //std::printf(" | %g %g %g\n", B(i), C(i), X(i));
    //}
}

/** ---- Test Jacobi Solver ---------------------------------------------------
 * test_linalg_jacobi<T>
 */
template<typename T>
void test_linalg_jacobi(const size_t ndim)
{
    /* Create random number generator */
    atto::math::rng::Kiss kiss(true);
    atto::math::rng::gauss<T> nrand;
    atto::math::rng::uniform<T> urand;

    /* Create a random matrix A */
    algo::Matrix<T> A(ndim, ndim);
    for (size_t i = 0; i < ndim; ++i) {
        for (size_t j = 0; j < ndim; ++j) {
            A(i,j) = nrand(kiss);
        }
        A(i,i) += 1.0 + urand(kiss);
    }

    /*
     * Make it symmetric by adding its transpose.
     */
    algo::Matrix<T> Atr(ndim, ndim);
    algo::transpose_matrix<T>(A, Atr);
    for (size_t i = 0; i < ndim; ++i) {
        for (size_t j = 0; j < ndim; ++j) {
            A(i,j) += Atr(i,j);
        }
    }

    /*
     * Solve the eigenvalue problem.
     */
    algo::Matrix<T> D(ndim, ndim);
    algo::Matrix<T> V(ndim, ndim);

    T maxeps = std::sqrt(std::numeric_limits<T>::epsilon());
    size_t maxiter = 1000000;

    if (ndim < 160) {
        REQUIRE(algo::eigen_jacobi<T>(A, D, V, maxeps, maxiter));
    } else {
        REQUIRE(algo::eigen_jacobi_omp<T>(A, D, V, maxeps, maxiter));
    }
    std::printf("%s: converged to maxeps %.15e\n", __FUNCTION__, maxeps);

    /*
     * Sort the eigenvalues from largest to lowest.
     */
    algo::eigen_sort<T>(D,V);

    /*
     * Compute the error.
     */
    algo::Matrix<T> AV(ndim, ndim);
    algo::matmul_matrix<T>(A, V, AV);

    algo::Matrix<T> VD(ndim, ndim);
    algo::matmul_matrix<T>(V, D, VD);

    double err = 0.0;
    for (size_t i = 0; i < ndim; ++i) {
        for (size_t j = 0; j < ndim; ++j) {
            err += static_cast<double>(atto::math::fabs<T>(AV(i,j) - VD(i,j)));
        }
    }
    err /= (double) ndim;
    std::printf("%s: error %.15e\n", __FUNCTION__, err);

    if (typeid(T) == typeid(double)) {
        REQUIRE(err < 1.0E-4);
    } else if (typeid(T) == typeid(float)) {
        REQUIRE(err < 1.0);
    }
}

#endif /* TEST_ALGO_NUMERIC_LINALG_H_ */
