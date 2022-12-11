/*
 * test-sort.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ALGO_SORT_SORT_H_
#define TEST_ALGO_SORT_SORT_H_

#include "atto/core/core.hpp"

/** ---- Test Sort Vector Generator --------------------------------------------
 */
template<typename T>
std::vector<T> generate_vector(size_t n)
{
    /*
     * Generate a vector of random elements in the interval [0,nmax]
     */
    std::random_device rd;      /* rng device */
    std::mt19937 rng(rd());     /* rng engine seeded with rd */

    const size_t nmax = std::max<double>(
        1, std::min<double>(n, std::numeric_limits<T>::max()));

    std::vector<T> v(nmax);
    for (size_t i = 0; i < v.size(); ++i) {
        std::uniform_int_distribution<size_t> dist(1, nmax-1);
        size_t n = (i + dist(rng))%nmax;
        v[i] = static_cast<T>(n);
    }

    /*
     * Shuffle the vector using Knuth algorithm.
     */
    for (size_t i = 0; i < v.size(); ++i) {
        std::uniform_int_distribution<size_t> dist(i, v.size()-1);
        size_t j = dist(rng);
        std::swap<T>(v[i], v[j]);
    }

    return v;
}


/** ---- Test Sort Vector Generator --------------------------------------------
 */
template<typename T, template<typename> class C = std::less>
bool validate_vector(std::vector<T> vec, C<T> cmp = C<T>())
{
    for (size_t i = 0; i < vec.size()-1; ++i) {
        if (!cmp(vec[i], vec[i+1])) {
            return false;
        }
    }

    return true;
}

#endif /* TEST_ALGO_SORT_SORT_H_ */
