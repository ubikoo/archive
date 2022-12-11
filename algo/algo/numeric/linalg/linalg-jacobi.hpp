/*
 * linalg-jacobi.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_LINALG_JACOBI_H_
#define ALGO_NUMERIC_LINALG_JACOBI_H_

#include "atto/core/core.hpp"
#include "atto/math/math.hpp"
#include "numeric/array/array.hpp"

namespace algo {

/** ---- Jacobi eigenvalue rotation functions ---------------------------------
 * Jacobi plane rotation functions.
 * @brief These functions apply a Jacobi plane rotation on the eigenvalue
 * matrix D and the eigenvector matrix V.
 *
 * Given a rotation matrix in the plane defined by the vector directions
 * corresponding to the columns p and q, the forward and backward plane
 * rotations are given by
 *
 * R^t(i,j) * D(j,k) * R(k,l)
 *
 * The forward rotation computes the product
 *
 * D(j,k) * R(k,l)
 *
 * and the backward rotation computes the product
 *
 * R^(i,j) * (D * R)(j,l)
 *
 * where R^t is the transpose of R
 *
 * R^t(i,j) = R(j,i)
 *
 * The angle a is assumed to be chosen such that
 *
 * D(p,q) = D(q,p) = 0
 *
 * after the two operations.
 * It is also assumed that matrix D is real and symmetric,
 *
 * D(p,q) = D(q,p)
 *
 * before the rotation.
 *
 * D(m,m) is the eigenvalue diagonal matrix
 * V(m,m) is the eigenvector column matrix
 */

/**
 * eigen_rot_forward<T>
 * @brief Apply a forward plane rotation on matrix R.
 */
template<typename T>
core_inline
void eigen_rot_forward(
    size_t p,
    size_t q,
    Matrix<T> & core_restrict(R),
    T a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    /*
     * Compute rotation parameters
     *
     * a = cos(2*phi) / sin(2*phi)
     * t = sin(phi) / cos(phi)
     * s = sin(phi)
     * c = cos(phi)
     */
    T t = atto::math::fsign<T>(a) /
            (atto::math::fabs<T>(a) +  std::sqrt(a*a + 1.0));
    T c = 1.0 / std::sqrt(t*t + 1.0);
    T s = c*t;

    /* Rotate columns p and q of matrix R */
    for (size_t i = 0; i < R.n1(); ++i) {
        T rip = c*R(i,p) - s*R(i,q);
        T riq = s*R(i,p) + c*R(i,q);

        R(i,p) = rip;
        R(i,q) = riq;
    }
}

/**
 * eigen_rot_backward<T>
 * @brief Apply a backward plane rotation on matrix R.
 */
template<typename T>
core_inline
void eigen_rot_backward(
    size_t p,
    size_t q,
    Matrix<T> & core_restrict(R),
    T a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    /*
     * Compute rotation parameters
     *
     * a = cos(2*phi) / sin(2*phi)
     * t = sin(phi) / cos(phi)
     * s = sin(phi)
     * c = cos(phi)
     */
    T t = atto::math::fsign<T>(a) /
        (atto::math::fabs<T>(a) + std::sqrt(a*a + 1.0));
    T c = 1.0 / std::sqrt(t*t + 1.0);
    T s = c*t;

    /* Rotate rows p and q of matrix R */
    for (size_t j = 0; j < R.n2(); ++j) {
        T rpj = c*R(p,j) - s*R(q,j);
        T rqj = s*R(p,j) + c*R(q,j);

        R(p,j) = rpj;
        R(q,j) = rqj;
    }
}


/** ---- Jacobi Eigenvalue solver (serial) ------------------------------------
 * eigen_jacobi
 * @brief Solve the eigenvalue problem of a symmetric matrix A.
 * Use using Jacobi cyclic algorithm by eliminating the off-diagonal elements
 * in a cyclic manner.
 *
 * m        linear dimension
 * A        symmetric matrix
 * D        eigenvalue diagonal matrix
 * V        eigenvector column matrix
 * maxeps   maximum off-diagonal epsilon
 * maxiter  maximum number of matrix iterations
 */
template<typename T = double>
core_inline
bool eigen_jacobi(
    Matrix<T> & core_restrict(A),
    Matrix<T> & core_restrict(D),
    Matrix<T> & core_restrict(V),
    T maxeps = std::sqrt(std::numeric_limits<double>::epsilon()),
    size_t maxiter = std::numeric_limits<size_t>::max())
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(A.n1() == A.n2() &&
                D.n1() == A.n1() &&
                D.n2() == A.n2() &&
                V.n1() == A.n1() &&
                V.n2() == A.n2(),
                "invalid dimensions");
    core_assert(maxeps > 0.0 && maxiter > 0, "invalid Jacobi parameters");

    /* Matrix linear dimension */
    const size_t m = A.n1();

    /*
     * Initialize the eigenvalue matrix D from symmetric matrix A.
     * D(i,j) = A(i,j)
     */
    for (size_t i = 0; i < D.n1(); ++i) {
        for (size_t j = 0; j < D.n2(); ++j) {
            D(i,j) = A(i,j);
        }
    }

    /*
     * Initialize the eigenvector matrix V from identity.
     * V(i,j) = delta(i,j)
     */
    for (size_t i = 0; i < V.n1(); ++i) {
        V(i,i) = 1.0;
        for (size_t j = i+1; j < V.n2(); ++j) {
            V(i,j) = 0.0;
            V(j,i) = 0.0;
        }
    }

    /*
     * Compute average size of a diagonal element.
     */
    T ssr = 0.0;
    for (size_t i = 0; i < m; ++i) {
        ssr += D(i,i) * D(i,i);
    }
    T maxssr = maxeps * std::sqrt(ssr / (T) m);

    /*
     * Jacobi cyclic algorithm
     *
     * A single iteration comprises a Jacobi sweep over the entire matrix
     * eliminating the off-diagonal elements by a pair of forward and
     * backward rotations for each element.
     *
     * For each elimination step, the eigenvector matrix is also updated.
     * The eigenvector matrix V is column based, where column-i is the
     * eigenvector-i, V(:,i) = v(i).
     *
     * Only a forward rotation of column-i and column-j needs to be applied.
     *
     * Keep iterating until the absolute value of all off-diagonal elements
     * in the matrix is smaller than the the tolarence value maxssr or
     * until the maximum number of iterations is reached.
     */
    T a;
    size_t iter = 0;
    size_t nrot = std::numeric_limits<size_t>::max();
    while (++iter < maxiter && nrot > 0) {
        /*
         * Single Jacobi sweep.
         */
        nrot = 0;
        for (size_t i = 0; i < m; ++i) {           /* row i */
            for (size_t j = i+1; j < m; ++j) {     /* column j */
                if (atto::math::fabs<T>(D(i,j)) > maxssr) {
                    nrot++;

                    /*
                     * Rotate matrix in the plane defined by
                     * column-i and and column-j.
                     *
                     * Compute angle tangent a such that
                     *
                     * D(i,j) = D(j,i) = 0
                     *
                     * after a pair of forward and backward rotations.
                     *
                     * a = cos(2*phi) / sin(2*phi)
                     * t = sin(phi) / cos(phi)
                     * s = sin(phi)
                     * c = cos(phi)
                     */
                    a = 0.5*(D(j,j) - D(i,i)) / D(i,j);

                    /* Eigenvalue matrix D forward and backward rotations */
                    eigen_rot_forward<T>(i, j, D, a);   /* column i and j */
                    eigen_rot_backward<T>(i, j, D, a);  /* row i and j */

                    /* Update eigenvector matrix V */
                    eigen_rot_forward<T>(i, j, V, a);   /* column i and j */
                }
            }
        }
    }

    /*
     * Check convergence of the Jacobi algorithm
     */
    return (iter < maxiter);
}


/** ---- Jacobi eigenvalue permutation function -------------------------------
 * eigen_init_permute
 * @brief Compute the Jacobi permutation matrix.
 * A single sweep is composed of m permutations.
 * For a given permutation k = [0,n-1],
 *
 * N(k) is the total number of independent indices l = [0, N(k)],
 *
 * i(k,l) = P(k,l)
 * j(k,l) = P(k,m-l-1)
 */
core_inline
void eigen_init_permute(Matrix<size_t> & core_restrict(P))
{
    core_assert(P.n1() == P.n2(), "invalid dimensions");

    /* Matrix linear dimension */
    const size_t m = P.n1();
    size_t i, j, k, l, n;

    for (k = 0; k < m-1; ++k) {
        l = 0;
        n = (size_t) std::floor(0.5*(m - k));
        for (i = 0; i < n; ++i) {
            j = m - k - i - 1;
            P(k, l) = i;
            P(k, m-l-1) = j;
            l++;
        }

        if (k > 1) {
            n = m - (size_t) std::ceil(0.5*k);
            for (i = m - k; i < n; ++i) {
                j = m + m - k - i - 1;
                P(k, l) = i;
                P(k, m-l-1) = j;
                l++;
            }
        }
    }

    l = 0;
    n = (size_t) std::floor(0.5*m);
    for (i = 1; i < n; ++i) {
        j = m - i;
        P(m-1, l) = i;
        P(m-1, m-l-1) = j;
        l++;
    }
}


/** ---- Jacobi Eigenvalue solver(parallel) -----------------------------------
 * eigen_jacobi_omp
 * @brief Solve the eigenvalue problem using Jacobi cyclic algorithm.
 * Eliminate the off-diagonal elements in a cyclic manner.
 *
 * m        linear dimension
 * A        symmetric matrix
 * D        eigenvalue matrix
 * V        eigenvector matrix
 * maxeps   maximum off-diagonal epsilon
 * maxiter  maximum number of matrix iterations
 */
template<typename T = double>
core_inline
bool eigen_jacobi_omp(
    Matrix<T> & core_restrict(A),
    Matrix<T> & core_restrict(D),
    Matrix<T> & core_restrict(V),
    T maxeps = std::sqrt(std::numeric_limits<double>::epsilon()),
    size_t maxiter = std::numeric_limits<size_t>::max())
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(A.n1() == A.n2() &&
                D.n1() == A.n1() &&
                D.n2() == A.n2() &&
                V.n1() == A.n1() &&
                V.n2() == A.n2(),
                "invalid dimensions");
    core_assert(maxeps > 0.0 && maxiter > 0, "invalid Jacobi parameters");

    /* Matrix linear dimension */
    const size_t m = A.n1();

    /*
     * Initialize the eigenvalue matrix D from symmetric matrix A.
     * D(i,j) = A(i,j)
     */
    copy_matrix<T>(A, D);

    /*
     * Initialize the eigenvector matrix V from identity.
     * V(i,j) = delta(i,j)
     */
    identity_matrix<T>(V);

    /*
     * Compute average size of a diagonal element.
     */
    T ssr = 0.0;
    core_pragma_omp(parallel default(none) shared(m, D, ssr))
    {
        T thr_ssr = 0.0;        /* local ssr */
        core_pragma_omp(for schedule(static))
        for (size_t i = 0; i < m; ++i) {
            thr_ssr += D(i,i) * D(i,i);
        }

        core_pragma_omp(critical)
        ssr += thr_ssr;
        /* omp critical */
    } /* omp parallel */
    T maxssr = maxeps * std::sqrt(ssr / (T) m);

    /*
     * Jacobi cyclic algorithm
     *
     * This is the parallel version of the Jacobi cyclic algorithm.
     * A single iteration comprises a Jacobi sweep over the entire
     * matrix eliminating the off-diagonal elements.
     *
     * The original cyclic version sweeps the matrix off-diagonal
     * elements by rows.
     *
     * The parallel version is obtained by the simultaneous annihilation
     * of several off-diagonal elements using a permutation matrix P that
     * collects the indices of the elements.
     * The permutation matrix P ensures that for each iteration, the set
     * of indices is independent, thereby avoiding overlaps of the matrix
     * rotations.
     */
    Vector<T> Q(m);             /* Permutation rotation angle */
    Matrix<size_t> P(m,m);      /* Permutation matrix */
    eigen_init_permute(P);

    size_t nrot = std::numeric_limits<size_t>::max();
    size_t iter = 0;
    size_t n = (size_t) std::floor(0.5*m);
    size_t k;
    core_pragma_omp( \
        parallel default(none) \
        shared(m, k, n, iter, maxiter, D, V, P, Q, nrot, maxssr))
    {
        while (iter < maxiter && nrot > 0) {
            /*
             * Incremenet permutation index.
             * Use an explicit barrier to ensure all threads
             * use the same permutation k-index below.
             */
            core_pragma_omp(barrier)
            core_pragma_omp(single)
            {
                k = iter % m;
                if (k == 0) {
                    nrot = 0;
                }
                ++iter;
            }

            /*
             * Compute rotation angles.
             *
             * The rotation vector Q collects the rotation
             * angles for this permutation row.
             *
             */
            size_t thr_nrot = 0;    /* local rotation counter */
            core_pragma_omp(for schedule (dynamic))
            for (size_t l = 0; l < n; ++l) {
                size_t i = P(k, l);
                size_t j = P(k, m-l-1);

                if (i < j && atto::math::fabs<T>(D(i,j)) > maxssr) {
                    thr_nrot++;

                    /*
                     * Rotate matrix in the plane defined by
                     * column-i and and column-j.
                     *
                     * Compute angle tangent a such that
                     *
                     * D[i][j] = D[j][i] = 0
                     *
                     * after a pair of forward and backward rotations.
                     *
                     * a = cos(2*phi) / sin(2*phi)
                     * t = sin(phi) / cos(phi)
                     * s = sin(phi)
                     * c = cos(phi)
                     */
                    Q(l) = 0.5*(D(j,j) - D(i,i)) / D(i,j);
                } else {
                    Q(l) = 0.0;
                }
            }

            /*
             * Update global rotation counter.
             */
            core_pragma_omp(critical)
            nrot += thr_nrot;
            /* omp critical */

            /*
             * Rotate forwards,
             */
            core_pragma_omp(for schedule (dynamic))
            for (size_t l = 0; l < n; ++l) {
                size_t i = P(k, l);
                size_t j = P(k, m-l-1);
                if (i < j && atto::math::fabs<T>(Q(l)) > 0.0) {
                    eigen_rot_forward<T>(i, j, D, Q(l));    /* column i and j */
                }
            }

            /*
             * Rotate backwards.
             */
            core_pragma_omp(for schedule (dynamic))
            for (size_t l = 0; l < n; ++l) {
                size_t i = P(k, l);
                size_t j = P(k, m-l-1);
                if (i < j && atto::math::fabs<T>(Q(l)) > 0.0) {
                    eigen_rot_backward<T>(i, j, D, Q(l));   /* row i and j */
                }
            }

            /*
             * Rotate forwards again and update eigenvectors.
             */
            core_pragma_omp(for schedule (dynamic))
            for (size_t l = 0; l < n; ++l) {
                size_t i = P(k, l);
                size_t j = P(k, m-l-1);
                if (i < j && atto::math::fabs<T>(Q(l)) > 0.0) {
                    eigen_rot_forward<T>(i, j, V, Q(l));    /* column i and j */
                }
            }
        }
    } /* omp parallel */

    /*
     * Check convergence of the Jacobi algorithm.
     */
    return (iter < maxiter);
}


/** ---- Eigenvalue/Eigenvector manipulation ----------------------------------
 * eigen_sort
 * @brief Sort the diagonal eigenvalue matrix D and column eigenvector matrix V.
 */
template<typename T = double>
core_inline
void eigen_sort(Matrix<T> & core_restrict(D), Matrix<T> & core_restrict(V))
{
    static_assert(std::is_floating_point<T>::value, "non floating point type");
    core_assert(D.n1() == D.n2() &&
                V.n1() == D.n1() &&
                V.n2() == D.n2(),
                "invalid dimensions");

    /*
     * Get the column vector L from the diagonal eigenvalue matrix D.
     */
    Vector<T> L(D.n1());
    for (size_t i = 0; i < D.n1(); ++i) {
        L(i) = D(i,i);
    }

    /*
     * Index sort the eigenvalue column vector L.
     */
    Vector<size_t> idx(D.n1());
    for (size_t i = 0; i < idx.n1(); ++i) {
        idx(i) = i;
    }

    auto comp = [&] (const size_t &i, const size_t &j) -> bool {
        return (L(i) > L(j));
    };
    std::sort(idx.data(), idx.data() + idx.n1(), comp);

    /*
     * Sort the eigenvalue diagonal matrix D column-wise.
     * This assumes the off-diagonal elements are zero.
     */
    for (size_t i = 0; i < D.n1(); ++i) {
        D(i,i) = L(idx(i));
    }

    /*
     * Sort the eigenvector matrix V column-wise.
     */
    Matrix<T> W(V.n1(),V.n2());
    for (size_t i = 0; i < W.n1(); ++i) {
        for (size_t j = 0; j < W.n2(); ++j) {
            W(i,j) = V(i,idx(j));
        }
    }
    V = std::move(W);
}

} /* namespace algo */

#endif /* ALGO_NUMERIC_LINALG_JACOBI_H_ */
