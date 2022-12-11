/*
 * test_vector.cpp
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
#include "test-vector.hpp"

/** ---- Test Vector Data Structure -------------------------------------------
 */
TEST_CASE("Vector") {
    SECTION("Test Vector API") {
        const size_t n = 128;        /* minimum vector size */

        /*
         * Test vector read/write
         */
        REQUIRE(test_vector_read_write<long double>(n*n*n));
        REQUIRE(test_vector_read_write<double>(n*n*n));
        REQUIRE(test_vector_read_write<float>(n*n));

        REQUIRE(test_vector_read_write<long long>(n*n*n));
        REQUIRE(test_vector_read_write<long>(n*n*n));
        REQUIRE(test_vector_read_write<int>(n*n));
        REQUIRE(test_vector_read_write<short>(n));
        REQUIRE(test_vector_read_write<char>(n));

        REQUIRE(test_vector_read_write<unsigned long long>(n*n*n));
        REQUIRE(test_vector_read_write<unsigned long>(n*n*n));
        REQUIRE(test_vector_read_write<unsigned int>(n*n));
        REQUIRE(test_vector_read_write<unsigned short>(n));
        REQUIRE(test_vector_read_write<unsigned char>(n));

        REQUIRE(test_vector_read_write<int64_t>(n*n*n));
        REQUIRE(test_vector_read_write<int32_t>(n*n));
        REQUIRE(test_vector_read_write<int16_t>(n));
        REQUIRE(test_vector_read_write<int8_t>(n));

        REQUIRE(test_vector_read_write<uint64_t>(n*n*n));
        REQUIRE(test_vector_read_write<uint32_t>(n*n));
        REQUIRE(test_vector_read_write<uint16_t>(n));
        REQUIRE(test_vector_read_write<uint8_t>(n));

        /*
         * Test vector copy/assign
         */
        REQUIRE(test_vector_copy_assign<long double>(n*n*n));
        REQUIRE(test_vector_copy_assign<double>(n*n*n));
        REQUIRE(test_vector_copy_assign<float>(n*n));

        REQUIRE(test_vector_copy_assign<long long>(n*n*n));
        REQUIRE(test_vector_copy_assign<long>(n*n*n));
        REQUIRE(test_vector_copy_assign<int>(n*n));
        REQUIRE(test_vector_copy_assign<short>(n));
        REQUIRE(test_vector_copy_assign<char>(n));

        REQUIRE(test_vector_copy_assign<unsigned long long>(n*n*n));
        REQUIRE(test_vector_copy_assign<unsigned long>(n*n*n));
        REQUIRE(test_vector_copy_assign<unsigned int>(n*n));
        REQUIRE(test_vector_copy_assign<unsigned short>(n));
        REQUIRE(test_vector_copy_assign<unsigned char>(n));

        REQUIRE(test_vector_copy_assign<int64_t>(n*n*n));
        REQUIRE(test_vector_copy_assign<int32_t>(n*n));
        REQUIRE(test_vector_copy_assign<int16_t>(n));
        REQUIRE(test_vector_copy_assign<int8_t>(n));

        REQUIRE(test_vector_copy_assign<uint64_t>(n*n*n));
        REQUIRE(test_vector_copy_assign<uint32_t>(n*n));
        REQUIRE(test_vector_copy_assign<uint16_t>(n));
        REQUIRE(test_vector_copy_assign<uint8_t>(n));
    }
}
