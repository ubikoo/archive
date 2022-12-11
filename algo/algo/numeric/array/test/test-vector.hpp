/*
 * test-vector.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ALGO_NUMERIC_VECTOR_H_
#define TEST_ALGO_NUMERIC_VECTOR_H_

#include "numeric/array/array.hpp"

/** ---- Test vector read and write -------------------------------------------
 */
template<typename T>
bool test_vector_read_write(size_t n)
{
    std::printf("%s %s\n", __PRETTY_FUNCTION__, typeid(T).name());

    /* Set the output format of type T */
    std::string filename(TestPrefix + "/out.vector." + typeid(T).name());

    /* Create a vector of type T */
    algo::Vector<T> v1(n);
    for (size_t i = 0; i < n; ++i) {
        v1(i) = static_cast<T>(i);
    }

    /* Write vector of type T */
    atto::core::FileOut fout(filename);
    v1.write(fout);
    fout.close();

    /* Read vector of type T */
    algo::Vector<T> v2(n);
    atto::core::FileIn fin(filename);
    v2.read(fin);
    fin.close();

    /* Check error */
    double err = 0.0;
    for (size_t i = 0; i < n; ++i) {
        err += static_cast<double>(v2(i)- v1(i));
    }

    return (err == static_cast<T>(0));
}

/** ---- Test vector copy assign -------------------------------------------
 */
template<typename T>
bool test_vector_copy_assign(size_t n)
{
    std::printf("%s %s\n", __PRETTY_FUNCTION__, typeid(T).name());

    /* Create a vector of type T */
    algo::Vector<T> v1(n);
    for (size_t i = 0; i < n; ++i) {
        v1(i) = static_cast<T>(i);
    }

    /* Copy vector of type T */
    algo::Vector<T> v2(v1);
    algo::Vector<T> v3 = v1;

    /* Check error */
    double err = 0.0;
    for (size_t i = 0; i < n; ++i) {
        err += static_cast<double>(v2(i)- v1(i));
        err += static_cast<double>(v3(i)- v1(i));
    }

    return (err == static_cast<T>(0));
}

#endif /* TEST_ALGO_NUMERIC_VECTOR_H_ */
