/*
 * test-tensor.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ALGO_NUMERIC_TENSOR_H_
#define TEST_ALGO_NUMERIC_TENSOR_H_

#include "numeric/array/array.hpp"

/** ---- Test tensor read and write -------------------------------------------
 */
template<typename T>
bool test_tensor_read_write(size_t n)
{
    std::printf("%s %s\n", __PRETTY_FUNCTION__, typeid(T).name());

    /* Set the output format of type T */
    std::string filename(TestPrefix + "/out.tensor." + typeid(T).name());

    /* Create a tensor of type T */
    algo::Tensor<T> v1(n,n,n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                v1(i,j,k) = static_cast<T>(i+j+k);
            }
        }
    }

    /* Write tensor of type T */
    atto::core::FileOut fout(filename);
    v1.write(fout);
    fout.close();


    /* Read tensor of type T */
    algo::Tensor<T> v2(n,n,n);
    atto::core::FileIn fin(filename);
    v2.read(fin);
    fin.close();

    /* Check error */
    double err = 0.0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                err += static_cast<double>(v2(i,j,k)- v1(i,j,k));
            }
        }
    }

    return (err == static_cast<T>(0));
}

/** ---- Test tensor copy assign ----------------------------------------------
 */
template<typename T>
bool test_tensor_copy_assign(size_t n)
{
    std::printf("%s %s\n", __PRETTY_FUNCTION__, typeid(T).name());

    /* Create a tensor of type T */
    algo::Tensor<T> v1(n,n,n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                v1(i,j,k) = static_cast<T>(i+j+k);
            }
        }
    }

    /* Copy tensor of type T */
    algo::Tensor<T> v2(v1);
    algo::Tensor<T> v3 = v1;


    /* Check error */
    double err = 0.0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                err += static_cast<double>(v2(i,j,k)- v1(i,j,k));
                err += static_cast<double>(v3(i,j,k)- v1(i,j,k));
            }
        }
    }

    return (err == static_cast<T>(0));
}

#endif /* TEST_ALGO_NUMERIC_TENSOR_H_ */
