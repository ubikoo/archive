/*
 * test.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "test-memory.hpp"
#include "test-string.hpp"
#include "test-file.hpp"

/** ---- Memory Tests ---------------------------------------------------------
 * main test client
 */
int main(int argc, char const *argv[])
{
    try {
        test_core_memory(256);
        test_core_memory(512);
        test_core_string();
        test_core_file();
    } catch (std::exception& e) {
        core_throw_(core::str::format("%s\nFAIL", e.what()));
        std::printf("exception: %s\n", e.what());
    }

    exit(EXIT_SUCCESS);
}
