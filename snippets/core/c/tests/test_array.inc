/*
 * test.inc
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 *
 *  See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ARRAY_TEST
#define ARRAY_TEST(Name,Suffix,Type) \
/** \
 * debug_array_##Suffix \
 * @brief Print array debug info \
 */ \
void debug_array_##Suffix (struct Array##Name *array) \
{ \
    printf("\n%s\n", __func__); \
    printf("%p\n", (void *) array); \
    printf("size %lu\n", array->size); \
    printf("capacity %lu\n", array->capacity); \
    printf("data %p\n", (void *) array->data); \
    printf("is_empty %s\n", \
            array_##Suffix##_is_empty(array) ? "true" : "false"); \
} \
\
/** \
 * test_array_##Suffix \
 */ \
void test_array_##Suffix (void) \
{ \
    { \
        struct Array##Name *arr = create_array_##Suffix(); \
        debug_array_##Suffix(arr); \
        core_assert_(array_##Suffix##_is_empty(arr), "FAIL\n"); \
        core_assert_(arr->size == 0, "FAIL\n"); \
        core_assert_(arr->capacity == 1, "FAIL\n"); \
\
        /* \
         * Insert 1 \
         */ \
        array_##Suffix##_push(arr, (Type) 1.0); \
        debug_array_##Suffix(arr); \
        core_assert_(!array_##Suffix##_is_empty(arr), "FAIL\n"); \
        core_assert_(arr->size == 1, "FAIL\n"); \
        core_assert_(arr->capacity == 1, "FAIL\n"); \
\
        /* \
         * Insert 2 \
         */ \
        array_##Suffix##_push(arr, (Type) 2.0); \
        debug_array_##Suffix(arr); \
        core_assert_(!array_##Suffix##_is_empty(arr), "FAIL\n"); \
        core_assert_(arr->size == 2, "FAIL\n"); \
        core_assert_(arr->capacity == 2, "FAIL\n"); \
\
        /* \
         * Insert 3 \
         */ \
        array_##Suffix##_push(arr, (Type) 3.0); \
        debug_array_##Suffix(arr); \
        core_assert_(!array_##Suffix##_is_empty(arr), "FAIL\n"); \
        core_assert_(arr->size == 3, "FAIL\n"); \
        core_assert_(arr->capacity == 4, "FAIL\n"); \
\
        /* \
         * Insert 4 \
         */ \
        array_##Suffix##_push(arr, (Type) 4.0); \
        debug_array_##Suffix(arr); \
        core_assert_(!array_##Suffix##_is_empty(arr), "FAIL\n"); \
        core_assert_(arr->size == 4, "FAIL\n"); \
        core_assert_(arr->capacity == 4, "FAIL\n"); \
\
        /* \
         * Insert 5 \
         */ \
        array_##Suffix##_push(arr, (Type) 5.0); \
        debug_array_##Suffix(arr); \
        core_assert_(!array_##Suffix##_is_empty(arr), "FAIL\n"); \
        core_assert_(arr->size == 5, "FAIL\n"); \
        core_assert_(arr->capacity == 8, "FAIL\n"); \
\
        /* \
         * Check we have the right numbers                                                                                                            C \
         */ \
        for (size_t i = 0; i < arr->size; ++i) { \
            core_assert_(arr->data[i] == (Type) (i+1), "FAIL\n"); \
        } \
\
        /* \
         * Remove 5 \
         */ \
        core_assert_(array_##Suffix##_pop(arr) == 5.0, "FAIL\n"); \
        core_assert_(arr->size == 4, "FAIL\n"); \
        core_assert_(arr->capacity == 8, "FAIL\n"); \
\
        /* \
         * Remove 4 \
         */ \
        core_assert_(array_##Suffix##_pop(arr) == 4.0, "FAIL\n"); \
        core_assert_(arr->size == 3, "FAIL\n"); \
        core_assert_(arr->capacity == 8, "FAIL\n"); \
\
        /* \
         * Remove 3 \
         */ \
        core_assert_(array_##Suffix##_pop(arr) == 3.0, "FAIL\n"); \
        core_assert_(arr->size == 2, "FAIL\n"); \
        core_assert_(arr->capacity == 4, "FAIL\n"); \
\
        /* \
         * Remove 2 \
         */ \
        core_assert_(array_##Suffix##_pop(arr) == 2.0, "FAIL\n"); \
        core_assert_(arr->size == 1, "FAIL\n"); \
        core_assert_(arr->capacity == 2, "FAIL\n"); \
\
        /* \
         * Remove 1 \
         */ \
        core_assert_(array_##Suffix##_pop(arr) == 1.0, "FAIL\n"); \
        core_assert_(arr->size == 0, "FAIL\n"); \
        core_assert_(arr->capacity == 2, "FAIL\n"); \
\
        destroy_array_##Suffix(arr); \
    } \
\
    { \
        struct Array##Name *arr = create_array_##Suffix(); \
        for (size_t i = 0; i < 128; ++i) { \
            array_##Suffix##_push(arr, (Type) i); \
            core_assert_(arr->size == i+1, "FAIL\n"); \
        } \
        array_##Suffix##_clear(arr); \
        core_assert_(arr->size == 0, "FAIL\n"); \
        core_assert_(arr->capacity == 1, "FAIL\n"); \
        core_assert_(array_##Suffix##_is_empty(arr), "FAIL\n"); \
        destroy_array_##Suffix(arr); \
    } \
}
#endif /* ARRAY_TEST */