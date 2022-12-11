/*
 * test-matrix.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ALGO_NUMERIC_MATRIX_H_
#define TEST_ALGO_NUMERIC_MATRIX_H_

#include "numeric/array/array.hpp"

/** ---- Test matrix read and write -------------------------------------------
 */
template<typename T>
bool test_matrix_read_write(size_t n)
{
    std::printf("%s %s\n", __PRETTY_FUNCTION__, typeid(T).name());

    /* Set the output format of type T */
    std::string filename(TestPrefix + "/out.matrix." + typeid(T).name());

    /* Create a matrix of type T */
    algo::Matrix<T> v1(n,n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            v1(i,j) = static_cast<T>(i+j);
        }
    }

    /* Write matrix of type T */
    atto::core::FileOut fout(filename);
    v1.write(fout);
    fout.close();


    /* Read matrix of type T */
    algo::Matrix<T> v2(n,n);
    atto::core::FileIn fin(filename);
    v2.read(fin);
    fin.close();

    /* Check error */
    double err = 0.0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            err += static_cast<double>(v2(i,j)- v1(i,j));
        }
    }

    return (err == static_cast<T>(0));
}

/** ---- Test matrix copy assign ----------------------------------------------
 */
template<typename T>
bool test_matrix_copy_assign(size_t n)
{
    std::printf("%s %s\n", __PRETTY_FUNCTION__, typeid(T).name());

    /* Create a matrix of type T */
    algo::Matrix<T> v1(n,n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            v1(i,j) = static_cast<T>(i+j);
        }
    }

    /* Copy matrix of type T */
    algo::Matrix<T> v2(v1);
    algo::Matrix<T> v3 = v1;

    /* Check error */
    double err = 0.0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            err += static_cast<double>(v2(i,j)- v1(i,j));
            err += static_cast<double>(v3(i,j)- v1(i,j));
        }
    }

    return (err == static_cast<T>(0));
}

#endif /* TEST_ALGO_NUMERIC_MATRIX_H_ */
