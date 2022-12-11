/*
 * test-memory.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "test-memory.hpp"

/** ---- Memory test structure ------------------------------------------------
 */
struct X {
    /* Member variables and access functions. */
    size_t num_ = 0;
    size_t id_ = 0;
    core::align_unique_array<size_t> arr_;

    int get() const { return num_; }
    void set(int num) { num_ = num; }

    /* Constructor and destructor. */
    X(size_t num, size_t id)
        : num_(num)
        , id_(id) {
        arr_ = std::move(core::make_align_unique_array<size_t>(num_, id_));
    }
    ~X() = default;

    /* Disable copy constructor/assignment operators */
    X(const X &other) = delete;
    X &operator=(const X &other) = delete;

    /* Methods */
    bool check(void) const {
        bool ok = true;
        for (size_t i = 0; i < num_; ++i) {
            ok &= (*arr_)[i] == id_;
        }
        return ok;
    }
};

/* ---- Memory ----------------------------------------------------------------
 */
void test_core_memory(size_t length)
{
    const size_t array_size = 1024;

    /*
     * Test align_unique_ptr
     */
    {
        std::vector<core::align_unique_ptr<X>> arena;
        for (size_t i = 0; i < length; ++i) {
            core::align_unique_ptr<X> px1 =
                core::make_align_unique_ptr<X>(array_size, 1);
            arena.push_back(std::move(px1));
            core_assert_(arena.back()->check(), "FAIL");
            //arena.pop_back();
        }

        std::printf("arena size before clear: %lu\n", arena.size());
        core_assert_(arena.size() == length, "FAIL");
        arena.clear();
        std::printf("arena size after clear: %lu\n", arena.size());
        core_assert_(arena.size() == 0, "FAIL");
    }

    /*
     * Test align_unique_array
     */
    {
        std::vector<core::align_unique_array<X>> array_arena;
        for (size_t i = 0; i < length; ++i) {
            core::align_unique_array<X> px2 =
                core::make_align_unique_array<X>(array_size, array_size, i);
            array_arena.push_back(std::move(px2));
            for (size_t j = 0; j < array_size; ++j) {
                core_assert_((*array_arena.back() + j)->check(), "FAIL");
                core_assert_((*array_arena.back())[j].check(), "FAIL");
            }
            //array_arena.pop_back();
        }

        std::printf(
            "array_arena size before clear: %lu\n",
            array_arena.size());
        core_assert_(array_arena.size() == length, "FAIL");
        array_arena.clear();
        std::printf(
            "array_arena size after clear: %lu\n",
            array_arena.size());
        core_assert_(array_arena.size() == 0, "FAIL");
    }

    /* Return OK */
    std::printf("OK\n");
}
