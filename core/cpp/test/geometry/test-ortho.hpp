/*
 * test-ortho.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_CORE_GEOMETRY_ORTHO_H_
#define TEST_CORE_GEOMETRY_ORTHO_H_

#include "test-common.hpp"

/* ---- Test Orthonormal set --------------------------------------------------
 */
core_inline_
bool check_ortho(const core::math::Ortho &ortho)
{
    /* Check norm */
    double norm_u = std::fabs(core::math::norm(ortho.u()));
    if (!core::math::isequal(norm_u, 1.0)) {
        std::fprintf(stderr, "norm_u %lf\n", norm_u);
        return false;
    }

    double norm_v = std::fabs(core::math::norm(ortho.v()));
    if (!core::math::isequal(norm_v, 1.0)) {
        std::fprintf(stderr, "norm_v %lf\n", norm_v);
        return false;
    }

    double norm_w = std::fabs(core::math::norm(ortho.w()));
    if (!core::math::isequal(norm_w, 1.0)) {
        std::fprintf(stderr, "norm_w %lf\n", norm_w);
        return false;
    }

    /* Check orthogonality */
    double dot_uv = std::fabs(core::math::dot(ortho.u(), ortho.v()));
    if (!core::math::isequal(dot_uv, 0.0)) {
        std::fprintf(stderr, "dot_uv %lf\n", dot_uv);
        return false;
    }

    double dot_uw = std::fabs(core::math::dot(ortho.u(), ortho.w()));
    if (!core::math::isequal(dot_uw, 0.0)) {
        std::fprintf(stderr, "dot_uw %lf\n", dot_uw);
        return false;
    }

    double dot_vw = std::fabs(core::math::dot(ortho.v(), ortho.w()));
    if (!core::math::isequal(dot_vw, 0.0)) {
        std::fprintf(stderr, "dot_vw %lf\n", dot_vw);
        return false;
    }

    /* Check cross products */
    double dot_wuv = core::math::dot(ortho.w(), cross(ortho.u(), ortho.v()));
    if (!core::math::isequal(dot_wuv, 1.0)) {
        std::fprintf(stderr, "dot_wuv %lf\n", dot_wuv);
        return false;
    }

    double dot_uvw = core::math::dot(ortho.u(), cross(ortho.v(), ortho.w()));
    if (!core::math::isequal(dot_uvw, 1.0)) {
        std::fprintf(stderr, "dot_uvw %lf\n", dot_uvw);
        return false;
    }

    double dot_vwu = core::math::dot(ortho.v(), cross(ortho.w(), ortho.u()));
    if (!core::math::isequal(dot_vwu, 1.0)) {
        std::fprintf(stderr, "dot_vwu %lf\n", dot_vwu);
        return false;
    }

    return true;
}

core_inline_
bool check_isnan(const core::math::vec3d &v)
{
    if (std::isnan(v(0)) || std::isnan(v(1)) || std::isnan(v(2))) {
        std::cout << "isnan " << v << "\n";
        return false;
    }
    return true;
}

/*
 * test_ortho
 */
core_inline_
void test_ortho(const size_t n_items)
{
    /*
     * Create an array of vectors and orthonormal axis
     */
    core::align_unique_array<core::math::vec3d> u_arr =
        core::make_align_unique_array<core::math::vec3d>(n_items);
    core::align_unique_array<core::math::vec3d> v_arr =
        core::make_align_unique_array<core::math::vec3d>(n_items);
    core::align_unique_array<core::math::vec3d> w_arr =
        core::make_align_unique_array<core::math::vec3d>(n_items);
    core::align_unique_array<core::math::Ortho> o_arr =
        core::make_align_unique_array<core::math::Ortho>(n_items);

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
            kiss_arr = core::math::make_array_rng<core::math::RngKiss>(
                num_threads);
            rng_arr = core::math::make_array_rng_dist(num_threads);
        }
    } /* omp parallel */


    /*
     * Test orthonormal operators
     */
    core_omp_( \
        parallel default(none) \
        shared(n_items, u_arr, v_arr, w_arr, o_arr, \
               kiss_arr, rng_arr, std::cerr))
    {
        size_t id = omp_get_thread_num();
        core::math::RngKiss *kiss = (*kiss_arr) + id;
        core::math::RngDist *rng = (*rng_arr) + id;

        core_omp_(for schedule(static))
        for (size_t ix = 0; ix < n_items; ++ix) {
            core::math::vec3d *u = (*u_arr) + ix;
            core::math::vec3d *v = (*v_arr) + ix;
            core::math::vec3d *w = (*w_arr) + ix;
            core::math::Ortho *o = (*o_arr) + ix;

            *u = core::math::vec3d((*rng).nrand(*kiss)*rand_sdev + rand_avg,
                                   (*rng).nrand(*kiss)*rand_sdev,
                                   (*rng).nrand(*kiss)*rand_sdev);

            *v = core::math::vec3d((*rng).nrand(*kiss)*rand_sdev,
                                   (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                                   (*rng).nrand(*kiss)*rand_sdev);

            *w = core::math::vec3d((*rng).nrand(*kiss)*rand_sdev,
                                   (*rng).nrand(*kiss)*rand_sdev,
                                   (*rng).nrand(*kiss)*rand_sdev + rand_avg);

            /* Check vector samples */
            REQUIRE(check_isnan(*u));
            REQUIRE(check_isnan(*v));
            REQUIRE(check_isnan(*w));

            /*
             * create_from_u
             */
            (*o).create_from_u(*u);
            if (!check_ortho(*o)) {
                std::cerr << "create_from_u\n";
                std::cerr << to_string(*o, "%lf") << "\n";
                std::cerr << to_string(*u, "%lf") << "\n";
            }
            REQUIRE(check_ortho(*o));

            /*
             * create_from_v
             */
            (*o).create_from_v(*v);
            if (!check_ortho(*o)) {
                std::cerr << "create_from_v\n";
                std::cerr << to_string(*o, "%lf") << "\n";
                std::cerr << to_string(*v, "%lf") << "\n";
            }
            REQUIRE(check_ortho(*o));

            /*
             * create_from_w
             */
            (*o).create_from_w(*w);
            if (!check_ortho(*o)) {
                std::cerr << "create_from_w\n";
                std::cerr << to_string(*o, "%lf") << "\n";
                std::cerr << to_string(*w, "%lf") << "\n";
            }
            REQUIRE(check_ortho(*o));

            /*
             * create_from_uv
             */
            (*o).create_from_uv(*u, *v);
            if (!check_ortho(*o)) {
                std::cerr << "create_from_uv\n";
                std::cerr << to_string(*o, "%lf") << "\n";
            }
            REQUIRE(check_ortho(*o));

            /*
             * create_from_vw
             */
            (*o).create_from_vw(*v, *w);
            if (!check_ortho(*o)) {
                std::cerr << "create_from_vw\n";
                std::cerr << to_string(*o, "%lf") << "\n";
            }
            REQUIRE(check_ortho(*o));

            /*
             * create_from_wu
             */
            (*o).create_from_wu(*w, *u);
            if (!check_ortho(*o)) {
                std::cerr << "create_from_wu\n";
                std::cerr << to_string(*o, "%lf") << "\n";
            }
            REQUIRE(check_ortho(*o));

            /*
             * world_to_local and local_to_world
             */
            core::math::vec3d a((*rng).nrand(*kiss)*rand_sdev + rand_avg,
                                (*rng).nrand(*kiss)*rand_sdev + rand_avg,
                                (*rng).nrand(*kiss)*rand_sdev + rand_avg);

            core::math::vec3d e =
                a - (*o).local_to_world((*o).world_to_local(a));
            if (!core::math::isequal(norm(e), 0.0)) {
                std::cerr << "world_to_local -> local_to_world\n";
                std::cerr << "o:\n" << to_string(*o, "%lf") << "\n";
                std::cerr << "a: " << to_string(a, "%lf") << "\n";
                std::cerr << "e: " << to_string(e, "%lf") << "\n";
                std::cerr << "" << "\n";
            }
            REQUIRE(core::math::isequal(norm(e), 0.0));
        }
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

#endif /* TEST_CORE_GEOMETRY_ORTHO_H_ */
