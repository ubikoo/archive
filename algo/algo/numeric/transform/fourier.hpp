/*
 * fourier.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_NUMERIC_FOURIER_H_
#define ALGO_NUMERIC_FOURIER_H_

#include "atto/core/core.hpp"
#include "numeric/array/array.hpp"

namespace algo {

/** ---- 1d Fourier transform functions ----------------------------------------
 * struct Dft1d
 *
 * 1d-dft forward transform dft of a real valued function
 * onto its complex counterpart,
 *
 * f(n) -> (fre(n), fim(n))
 *
 * and corresponding inverse transform idft,
 *
 * (fre(n), fim(n)) -> f(n)
 *
 * n = number of waves
 * f = real function
 * fre = transform real part
 * fim = transform imaginary part
 */
template<typename T = double, bool IsPar = true>
struct Dft1d {
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    /* 1d-dft constructor/destructor. */
    Dft1d() = default;
    ~Dft1d() = default;

    /*
     * dft, dft_transform
     * 1d Forward discrete Fourier transform.
     */
    void dft_transform(size_t m1,
                       Vector<T> & core_restrict(f),
                       Vector<T> & core_restrict(fre),
                       Vector<T> & core_restrict(fim))
    {
        const size_t n = f.n1();
        double phi, k = 2.0 * M_PI / (T) n;

        fre(m1) = 0.0;
        fim(m1) = 0.0;
        for (size_t n1 = 0; n1 < n; ++n1) {
            phi = k * (T) m1*n1;

            fre(m1) += f(n1) * std::cos(phi);
            fim(m1) -= f(n1) * std::sin(phi);
        }
    }

    void dft(Vector<T> & core_restrict(f),
              Vector<T> & core_restrict(fre),
              Vector<T> & core_restrict(fim))
    {
        core_assert(f.n1() == fre.n1() &&
                     f.n1() == fim.n1(),
                     "invalid dimensions");

        const size_t n = f.n1();

        core_pragma_omp(parallel for if(IsPar) default(none) \
            shared(f, fre, fim) schedule(static))
        for (size_t m1 = 0; m1 < n; ++m1) {
            dft_transform(m1, n, f, fre, fim);
        }
    }

    /*
     * idft, idft_transform
     * 1d Inverse discrete Fourier transform
     */
    void idft_transform(size_t n1,
                        Vector<T> & core_restrict(fre),
                        Vector<T> & core_restrict(fim),
                        Vector<T> & core_restrict(f))
   {
        const size_t n = f.n1();
        double phi, k = 2.0 * M_PI / (T) n;

        f(n1) = 0.0;
        for (size_t m1 = 0; m1 < n; ++m1) {
            phi = k * (T) m1*n1;
            f(n1) += fre(m1) * std::cos(phi) - fim(m1) * std::sin(phi);
        }
        f(n1) /= (T) n;
    }

    void idft(Vector<T> & core_restrict(fre),
               Vector<T> & core_restrict(fim),
               Vector<T> & core_restrict(f))
    {
        core_assert(f.n1() == fre.n1() && f.n1() == fim.n1(),
                    "invalid dimensions");

        const size_t n = f.n1();

        core_pragma_omp(parallel for if(IsPar) default(none) \
            shared(f, fre, fim) schedule(static))
        for (size_t n1 = 0; n1 < n; ++n1) {
            idft_transform(n1, n, fre, fim, f);
        }
    }
};


/** ---- 2d Fourier transform functions ----------------------------------------
 * struct Dft2d
 *
 * 2d-dft forward transform dft of a real valued function
 * onto its complex counterpart,
 *
 * f(n) -> (fre(n), fim(n))
 *
 * and corresponding inverse transform idft,
 *
 * (fre(n), fim(n)) -> f(n)
 *
 * n = number of waves
 * f = real function
 * fre = transform real part
 * fim = transform imaginary part
 */
template<typename T = double, bool IsPar = true>
struct Dft2d {
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    /* 2d-dft constructor/destructor. */
    Dft2d() = default;
    ~Dft2d() = default;

    /*
     * dft, dft_transform
     * 2d Forward discrete Fourier transform.
     */
    void dft_transform(size_t m1,
                       Matrix<T> & core_restrict(f),
                       Matrix<T> & core_restrict(fre),
                       Matrix<T> & core_restrict(fim))
    {
        const size_t n = f.n1();
        double phi, k = 2.0 * M_PI / (T) n;

        for (size_t m2 = 0; m2 < n; ++m2) {
            fre(m1, m2) = 0.0;
            fim(m1, m2) = 0.0;

            for (size_t n1 = 0; n1 < n; ++n1) {
                for (size_t n2 = 0; n2 < n; ++n2) {
                    phi = k * (T) m1*n1 +
                          k * (T) m2*n2;

                    fre(m1, m2) += f(n1, n2) * std::cos(phi);
                    fim(m1, m2) -= f(n1, n2) * std::sin(phi);
                }
            }
        }
    }

    void dft(Matrix<T> & core_restrict(f),
              Matrix<T> & core_restrict(fre),
              Matrix<T> & core_restrict(fim))
    {
        core_assert(f.n1() == f.n2()    &&
                    f.n1() == fre.n1()  &&
                    f.n2() == fre.n2()  &&
                    f.n1() == fim.n1()  &&
                    f.n2() == fim.n2(),
                    "invalid dimensions");

        const size_t n = f.n1();

        core_pragma_omp(parallel for if(IsPar) default(none) \
            shared(f, fre, fim) schedule(static))
        for (size_t m1 = 0; m1 < n; ++m1) {
            dft_transform(m1, n, f, fre, fim);
        }
    }

    /*
     * idft, idft_transform
     * 2d Inverse discrete Fourier transform.
     */
    void idft_transform(size_t n1,
                        Matrix<T> & core_restrict(fre),
                        Matrix<T> & core_restrict(fim),
                        Matrix<T> & core_restrict(f))
   {
        const size_t n = f.n1();
        double phi, k = 2.0 * M_PI / (T) n;

        for (size_t n2 = 0; n2 < n; ++n2) {
            f(n1, n2) = 0.0;
            for (size_t m1 = 0; m1 < n; ++m1) {
                for (size_t m2 = 0; m2 < n; ++m2) {
                    phi = k * (T) m1*n1 +
                          k * (T) m2*n2;

                    f(n1, n2) += fre(m1, m2) * std::cos(phi) -
                                 fim(m1, m2) * std::sin(phi);
                }
            }
            f(n1, n2) /= (T) n * n;
        }
    }

    void idft(Matrix<T> & core_restrict(fre),
               Matrix<T> & core_restrict(fim),
               Matrix<T> & core_restrict(f))
    {
        core_assert(f.n1() == f.n2()    &&
                    f.n1() == fre.n1()  &&
                    f.n2() == fre.n2()  &&
                    f.n1() == fim.n1()  &&
                    f.n2() == fim.n2(),
                    "invalid dimensions");

        const size_t n = f.n1();

        core_pragma_omp(parallel for if(IsPar) default(none) \
            shared(f, fre, fim) schedule(static))
        for (size_t n1 = 0; n1 < n; ++n1) {
            idft_transform(n1, n, fre, fim, f);
        }
    }
};


/** ---- 3d Fourier transform functions ----------------------------------------
 * struct Dft3d
 *
 * 3d-dft forward transform dft of a real valued function
 * onto its complex counterpart,
 *
 * f(n) -> (fre(n), fim(n))
 *
 * and corresponding inverse transform idft,
 *
 * (fre(n), fim(n)) -> f(n)
 *
 * n = number of waves
 * f = real function
 * fre = transform real part
 * fim = transform imaginary part
 */
template<typename T = double, bool IsPar = true>
struct Dft3d {
    static_assert(std::is_floating_point<T>::value, "non floating point type");

    /* 3d-dft constructor/destructor. */
    Dft3d() = default;
    ~Dft3d() = default;

    /*
     * dft, dft_transform
     * 3d Forward discrete Fourier transform.
     */
    void dft_transform(size_t m1,
                       Tensor<T> & core_restrict(f),
                       Tensor<T> & core_restrict(fre),
                       Tensor<T> & core_restrict(fim))
   {
        const size_t n = f.n1();
        double phi, k = 2.0 * M_PI / (T) n;

        for (size_t m2 = 0; m2 < n; ++m2) {
            for (size_t m3 = 0; m3 < n; ++m3) {
                fre(m1, m2, m3) = 0.0;
                fim(m1, m2, m3) = 0.0;

                for (size_t n1 = 0; n1 < n; ++n1) {
                    for (size_t n2 = 0; n2 < n; ++n2) {
                        for (size_t n3 = 0; n3 < n; ++n3) {
                            phi = k * (T) m1*n1 +
                                  k * (T) m2*n2 +
                                  k * (T) m3*n3;

                            fre(m1, m2, m3) += f(n1, n2, n3) * std::cos(phi);
                            fim(m1, m2, m3) -= f(n1, n2, n3) * std::sin(phi);
                        }
                    }
                }
            }
        }
    }

    void dft(Tensor<T> & core_restrict(f),
             Tensor<T> & core_restrict(fre),
             Tensor<T> & core_restrict(fim))
   {
        core_assert(f.n1() == f.n2()   &&
                    f.n1() == f.n3()   &&
                    f.n1() == fre.n1() &&
                    f.n2() == fre.n2() &&
                    f.n3() == fre.n3() &&
                    f.n1() == fim.n1() &&
                    f.n2() == fim.n2() &&
                    f.n3() == fim.n3(),
                    "invalid dimensions");

        const size_t n = f.n1();

        core_pragma_omp(parallel for if(IsPar) default(none) \
            shared(f, fre, fim) schedule(static))
        for (size_t m1 = 0; m1 < n; ++m1) {
            dft_transform(m1, n, f, fre, fim);
        }
    }

    /*
     * idft, idft_transform
     * 3d Inverse discrete Fourier transform.
     */
    void idft_transform(size_t n1,
                        Tensor<T> & core_restrict(fre),
                        Tensor<T> & core_restrict(fim),
                        Tensor<T> & core_restrict(f))
   {
        const size_t n = f.n1();
        double phi, k = 2.0 * M_PI / (T) n;

        for (size_t n2 = 0; n2 < n; ++n2) {
            for (size_t n3 = 0; n3 < n; ++n3) {
                f(n1, n2, n3) = 0.0;
                for (size_t m1 = 0; m1 < n; ++m1) {
                    for (size_t m2 = 0; m2 < n; ++m2) {
                        for (size_t m3 = 0; m3 < n; ++m3) {
                            phi = k * (T) m1*n1 +
                                  k * (T) m2*n2 +
                                  k * (T) m3*n3;

                            f(n1, n2, n3) += fre(m1, m2, m3) * std::cos(phi) -
                                             fim(m1, m2, m3) * std::sin(phi);
                        }
                    }
                }
                f(n1, n2, n3) /= (T) n * n * n;
            }
        }
    }

    void idft(Tensor<T> & core_restrict(fre),
              Tensor<T> & core_restrict(fim),
              Tensor<T> & core_restrict(f))
    {
        core_assert(f.n1() == f.n2()   &&
                     f.n1() == f.n3()   &&
                     f.n1() == fre.n1() &&
                     f.n2() == fre.n2() &&
                     f.n3() == fre.n3() &&
                     f.n1() == fim.n1() &&
                     f.n2() == fim.n2() &&
                     f.n3() == fim.n3(),
                     "invalid dimensions");

        const size_t n = f.n1();

        core_pragma_omp(parallel for if(IsPar) default(none) \
            shared(f, fre, fim) schedule(static))
        for (size_t n1 = 0; n1 < n; ++n1) {
            idft_transform(n1, n, fre, fim, f);
        }
    }
};

} /* namespace algo */

#endif /* ALGO_NUMERIC_FOURIER_H_ */
