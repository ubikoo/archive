/*
 * is_ordered.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_SORT_IS_ORDERED_H_
#define ALGO_SORT_IS_ORDERED_H_

#include "atto/core/core.hpp"

namespace algo {

/** ---- is_ordered implementation --------------------------------------------
 *
 * is_ordered<Item>
 * @brief Return true if all array elements are ordered according
 * the binary comparator:
 *
 *  comp(a[i], a[i+1]) = true, i=lo..hi.
 *
 * Return false otherwise.
 *
 * @see Algorithms, 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
template<typename Item>
bool is_ordered(
    std::vector<Item> &arr,
    size_t lo,
    size_t hi,
    std::function<bool(const Item &, const Item &)> const &comp)
{
    core_assert(arr.size() > 0, "invalid array size");
    core_assert(lo >= 0 && lo < arr.size(), "invalid lower limit");
    core_assert(hi >= 0 && hi < arr.size(), "invalid upper limit");
    core_assert(lo <= hi, "invalid range");

    for (size_t i = lo; i < hi; ++i) {
        if (!comp(arr[i], arr[i+1])) {
            return false;
        }
    }

    return true;
}

} /* namespace algo */

#endif /* ALGO_SORT_IS_ORDERED_H_ */
