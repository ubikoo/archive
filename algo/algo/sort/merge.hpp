/*
 * merge.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_SORT_MERGE_H_
#define ALGO_SORT_MERGE_H_

#include "atto/core/core.hpp"
#include "algo/sort/is-ordered.hpp"

namespace algo {

/** ---- Merge implementation -------------------------------------------------
 *
 * merge<Item>
 * @brief Merge a[lo..mid] with a[mid+1 ..hi] into aux[lo .. hi].
 * It is assumed that the following precondition is valid:
 *
 *  comp(a[i], a[i+1]) = true
 *
 * for i=[lo..mid-1] and for i=[mid+1..hi-1].
 *
 * For example if comp = less, then it is assumed that
 * a[lo .. mid] and a[mid+1 .. hi] are sorted subarrays:
 *
 *  a[i] < a[i+1]   for i=lo..mid-1 and for i=mid+1..hi-1
 *
 * @see Algorithms, 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
template<typename Item>
void merge(
    std::vector<Item> &arr,
    std::vector<Item> &aux,
    size_t lo,
    size_t mid,
    size_t hi,
    std::function<bool(const Item &, const Item &)> const &comp)
{
    core_assert(arr.size() > 0 && aux.size() > 0, "invalid sizes");
    core_assert(arr.size() == aux.size(), "unequal sizes");
    core_assert(lo <= mid && mid <= hi, "invalid range");

    /*
     * Precondition: a[lo..mid] and a[mid+1..hi] are sorted subarrays.
     */
#if 1
     core_assert(is_ordered<Item>(arr, lo, mid, comp), "unordered array");
     core_assert(is_ordered<Item>(arr, mid+1, hi, comp), "unordered array");
#endif

    /*
     * Copy the elements to aux.
     */
    for (size_t k = lo; k <= hi; ++k) {
        aux[k] = arr[k];
    }

    /*
     * Merge back to arr.
     */
    size_t i = lo;
    size_t j = mid + 1;
    for (size_t k = lo; k <= hi; ++k) {
        if (i > mid) {          /* exhaust a[lo..mid] - copy upper range */
            arr[k] = aux[j++];
        } else if (j > hi) {    /* exhaust a[mid+1..hi] - copy lower range */
            arr[k] = aux[i++];
        } else if (comp(aux[i], aux[j])) {  /* order(a[i], b[j]), copy a[i] */
            arr[k] = aux[i++];
        } else {                            /* otherwise copy a[j] */
            arr[k] = aux[j++];
        }
    }

    /*
     * Postcondition: a[lo..hi] is sorted.
     */
#if 1
     core_assert(is_ordered<Item>(arr, lo, hi, comp), "unordered array");
#endif
}

} /* namespace algo */

#endif /* ALGO_SORT_MERGE_H_ */
