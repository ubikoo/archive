/*
 * test-performance-vec3.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_PERFORMANCE_VEC3_H_
#define TEST_CORE_GEOMETRY_PERFORMANCE_VEC3_H_

#include "test-common.hpp"

/** ---------------------------------------------------------------------------
 * test_performance_vec3
 * @brief vec3d performance test.
 */
core_inline_
void test_performance_vec3 (const size_t n_items, const double maxeps)
{
    const size_t dim = 3;
    const core::math::vec3d zero(0.0);
    const core::math::vec3d one(1.0);
    const core::math::vec3d two(2.0);

    /*
     * Create an array of vectors
     */
    core::align_unique_array<core::math::vec3d> a_arr =
        core::make_align_unique_array<core::math::vec3d>(n_items, 1.0);
    core::align_unique_array<core::math::vec3d> b_arr =
        core::make_align_unique_array<core::math::vec3d>(n_items, 2.0);
    core::align_unique_array<core::math::vec3d> c_arr =
        core::make_align_unique_array<core::math::vec3d>(n_items, 0.0);

    /*
     * Create an array of random number generators
     */
    core::align_unique_array<core::math::RngKiss> kiss_arr;
    core::align_unique_array<core::math::RngDist> rng_arr;
    core_omp_(parallel default(none) shared(kiss_arr, rng_arr))
    {
        core_omp_(master)
        {
            int num_threads = omp_get_num_threads();
            kiss_arr = core::math::make_array_rng<core::math::RngKiss>(num_threads);
            rng_arr = core::math::make_array_rng_dist(num_threads);
        }
    } /* omp parallel */


    /*
     * Test vector operators
     */
    core_omp_( \
        parallel for default(none) \
        shared(n_items, a_arr, b_arr, c_arr, kiss_arr, rng_arr) \
        schedule(static))
    for (size_t ix = 0; ix < n_items; ++ix) {
        core::math::vec3d *a = (*a_arr) + ix;
        core::math::vec3d *b = (*b_arr) + ix;
        core::math::vec3d *c = (*c_arr) + ix;

        size_t id = omp_get_thread_num();
        core::math::RngKiss *kiss = (*kiss_arr) + id;
        core::math::RngDist *rng = (*rng_arr) + id;

        *a = core::math::vec3d((*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg);
        *b = (*a) * (-1.0);

        /*
         * Test arithmetic functions
         */
        (*c) = (*a) + (*b);   /* 1 + (-1) = 0 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(core::math::isequal((*c)(j), 0.0));
        }

        (*c) = (*a) - (*b);   /* 1 - (-1) = 2 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(core::math::isequal((*c)(j), 2*(*a)(j)));
        }

        (*c) = (*a) * (*b);   /* 1 * (-1) = -1 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(core::math::isequal((*c)(j), -(*a)(j)*(*a)(j)));
        }

        (*c) = (*a) / (*b);   /* 1 / (-1) = -1 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(core::math::isequal((*c)(j), -1.0));
        }

        /*
         * Test arithmetic assignment functions
         */
        ((*c) = (*a)) += (*b);   /* 1 + (-1) = 0 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(core::math::isequal((*c)(j), 0.0));
        }

        ((*c) = (*a)) -= (*b);   /* 1 - (-1) = 2 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(core::math::isequal((*c)(j), 2*(*a)(j)));
        }

        ((*c) = (*a)) *= (*b);   /* 1 * (-1) = -1 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(core::math::isequal((*c)(j), -(*a)(j)*(*a)(j)));
        }

        ((*c) = (*a)) /= (*b);   /* 1 / (-1) = -1 */
        for (size_t j = 0; j < dim; ++j) {
            REQUIRE(core::math::isequal((*c)(j), -1.0));
        }

        /* Test dot, norm and normalize */
        (*c) = (*a) - (*b);

        REQUIRE(core::math::isequal(dot(*c, *c), 4.0*dot(*a, *a)));
        REQUIRE(core::math::isequal(norm(*c), 2.0*std::sqrt(dot(*a, *a))));
        REQUIRE(core::math::isequal(norm(normalize(*c)), 1.0));
    }
}

/*
 * C linkage declarations
 */
#undef  __BEGIN_DECLS
#undef  __END_DECLS
#ifdef  __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS   /* empty */
#define __END_DECLS     /* empty */
#endif
__BEGIN_DECLS
__END_DECLS

#endif /* TEST_CORE_GEOMETRY_PERFORMANCE_VEC3_H_ */
