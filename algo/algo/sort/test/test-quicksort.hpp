/*
 * test-quicksort.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ALGO_SORT_QUICKSORT_H_
#define TEST_ALGO_SORT_QUICKSORT_H_

#include "algo/sort/sort.hpp"

/** ---- Test sort read and write ----------------------------------------------
 */
template<typename T>
void test_quicksort(size_t n)
{
    std::printf("%s %s\n", __PRETTY_FUNCTION__, typeid(T).name());

    /*
     * Create a vector of type T and the index vector
     */
    std::vector<T> v = generate_vector<T>(n);

    /*
     * Test Sort
     */
    algo::sort<T, std::less_equal, algo::QuickSort>(v);
    for (size_t i = 0; i < v.size()-1; ++i) {
        REQUIRE(v[i] <= v[i+1]);                 /* sort in decreasing order */
    }

    algo::sort<T, std::greater_equal, algo::QuickSort>(v);
    for (size_t i = 0; i < v.size()-1; ++i) {
        REQUIRE(v[i] >= v[i+1]);                 /* sort in increasing order */
    }

    /*
     * Test ArgSort
     */
    std::vector<size_t> ltix =
        algo::argsort<T, std::less_equal, algo::QuickSort>(v);
    for (size_t i = 0; i < v.size()-1; ++i) {
        REQUIRE(v[ltix[i]] <= v[ltix[i+1]]);     /* sort in decreasing order */
    }

    std::vector<size_t> gtix =
        algo::argsort<T, std::greater_equal, algo::QuickSort>(v);
    for (size_t i = 0; i < v.size()-1; ++i) {
        REQUIRE(v[gtix[i]] >= v[gtix[i+1]]);     /* sort in increasing order */
    }
}

#endif /* TEST_ALGO_SORT_QUICKSORT_H_ */
