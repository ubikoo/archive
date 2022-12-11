/*
 * test-performance-mat4.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_PERFORMANCE_MAT4_H_
#define TEST_CORE_GEOMETRY_PERFORMANCE_MAT4_H_

#include "test-common.hpp"

/** ---------------------------------------------------------------------------
 * test_performance_mat4
 * @brief mat4d performance test.
 */
core_inline_
void test_performance_mat4 (const size_t n_items, const double maxeps)
{
    const size_t dim = 4;
    const core::math::mat4d zero(0.0);
    const core::math::mat4d one(1.0);
    const core::math::mat4d two(2.0);

    /*
     * Create an array of mattors
     */
    core::align_unique_array<core::math::mat4d> a_arr =
        core::make_align_unique_array<core::math::mat4d>(n_items, 1.0);
    core::align_unique_array<core::math::mat4d> b_arr =
        core::make_align_unique_array<core::math::mat4d>(n_items, 2.0);
    core::align_unique_array<core::math::mat4d> c_arr =
        core::make_align_unique_array<core::math::mat4d>(n_items, 0.0);

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
     * Test mattor operators
     */
    core_omp_( \
        parallel for default(none) \
        shared(n_items, a_arr, b_arr, c_arr, kiss_arr, rng_arr) \
        schedule(static))
    for (size_t ix = 0; ix < n_items; ++ix) {
        core::math::mat4d *a = (*a_arr) + ix;
        core::math::mat4d *b = (*b_arr) + ix;
        core::math::mat4d *c = (*c_arr) + ix;

        size_t id = omp_get_thread_num();
        core::math::RngKiss *kiss = (*kiss_arr) + id;
        core::math::RngDist *rng = (*rng_arr) + id;

        *a = core::math::mat4d((*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg);
        *b = (*a) * (-1.0);

        /*
         * Test arithmetic functions
         */
        (*c) = (*a) + (*b);   /* 1 + (-1) = 0 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(core::math::isequal((*c)(j, k), 0.0));
            }
        }

        (*c) = (*a) - (*b);   /* 1 - (-1) = 2 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(core::math::isequal((*c)(j, k), 2*(*a)(j, k)));
            }
        }

        (*c) = (*a) * (*b);   /* 1 * (-1) = -1 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(core::math::isequal(
                    (*c)(j, k), -(*a)(j, k)*(*a)(j, k)));
            }
        }

        (*c) = (*a) / (*b);   /* 1 / (-1) = -1 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(core::math::isequal((*c)(j, k), -1.0));
            }
        }

        /*
         * Test arithmetic assignment functions
         */
        ((*c) = (*a)) += (*b);   /* 1 + (-1) = 0 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(core::math::isequal((*c)(j, k), 0.0));
            }
        }

        ((*c) = (*a)) -= (*b);   /* 1 - (-1) = 2 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(core::math::isequal((*c)(j, k), 2*(*a)(j, k)));
            }
        }

        ((*c) = (*a)) *= (*b);   /* 1 * (-1) = -1 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(core::math::isequal(
                    (*c)(j, k), -(*a)(j, k)*(*a)(j, k)));
            }
        }

        ((*c) = (*a)) /= (*b);   /* 1 / (-1) = -1 */
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(core::math::isequal((*c)(j, k), -1.0));
            }
        }

        /*
         * Test algebraic operators
         */
        *a = core::math::mat4d((*rng).nrand(*kiss)*rand_sdev + 2.0*rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + 2.0*rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + 2.0*rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                               (*rng).nrand(*kiss)*rand_sdev + 2.0*rand_avg);

        /* Matrix transpose */
        *b = transpose(*a);         /* b = a^t */
        *c = multiply(*a, *b);      /* c = a*a^t */
        double det_a = determinant(*a);
        double det_b = determinant(*b);
        double det_c = determinant(*c);
        REQUIRE(core::math::isequal(det_a, det_b));
        REQUIRE(core::math::isequal(det_a*det_b, det_c));

        /* Matrix inverse */
        *b = inverse(*a);           /* b = a^(-1) */
        *c = multiply(*a, *b);      /* c = a*a^(-1) */

        core::math::mat4d ident = identity(*c);
        for (size_t j = 0; j < dim; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                REQUIRE(core::math::isequal((*c)(j,k), ident(j,k)));
            }
        }

        /* Matrix solve */
        core::math::vec4d vec_b((*rng).nrand(*kiss)*rand_sdev + rand_avg,
                                (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                                (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                                (*rng).nrand(*kiss)*rand_sdev + rand_avg);
        core::math::mat4d inv_a = inverse(*a);
        /*
         * x = a^-1*b;
         * err = a*x - b
         */
        core::math::vec4d vec_x = multiply(inverse(*a), vec_b);
        core::math::vec4d err = vec_b - multiply(*a, vec_x);
        REQUIRE(core::math::isequal(norm(err), 0.0));
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

#endif /* TEST_CORE_GEOMETRY_PERFORMANCE_MAT4_H_ */
