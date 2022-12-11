/*
 * linalg.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "Catch2/catch.hpp"
#include "test/common.hpp"
#include "test-linalg.hpp"


/** ---- Test Linear Algebra Algorithm -----------------------------------------
 */
TEST_CASE("Linear Algebra") {
    const size_t n = 5;
    const std::array<size_t, n> ndim = {256, 512, 1024, 2048, 4096};
    const std::array<size_t, n> jdim = {64, 128, 256, 512, 1024};
    const std::array<size_t, n> iter = {64, 16, 8, 2, 1};

    /*
     * Test Gauss elimination solver
     */
    SECTION("Gauss solver") {
        /*
         * Gauss solver
         */
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < iter[i]; ++j) {
                std::printf("test_linalg_gauss<double> "
                            "dim = %lu, run %lu\n", ndim[i], iter[i]);
                test_linalg_gauss<double>(ndim[i]);
            }
        }

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < iter[i]; ++j) {
                std::printf("test_linalg_gauss<float> "
                            "dim = %lu, run %lu\n", ndim[i], iter[i]);
                test_linalg_gauss<float>(ndim[i]);
            }
        }
    }

    /*
     * Test Jacobi eigenvalue solver
     */
    SECTION("Jacobi solver") {
        /*
         * Jacobi solver.
         */
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < iter[i]; ++j) {
                std::printf("test_linalg_jacobi<double> "
                            "dim = %lu, run %lu\n", jdim[i], iter[i]);
                test_linalg_jacobi<double>(jdim[i]);
            }
        }

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < iter[i]; ++j) {
                std::printf("test_linalg_jacobi<float> "
                            "dim = %lu, run %lu\n", jdim[i], iter[i]);
                test_linalg_jacobi<float>(jdim[i]);
            }
        }
    }
}
