/*
 * test_array.c
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 *
 *  See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "test_array.h"
#include "test_array.inc"

ARRAY_TEST(LongDouble, ldouble, long double)
ARRAY_TEST(Double, double, double)
ARRAY_TEST(Float, float, float)

ARRAY_TEST(LongLong, llong, long long)
ARRAY_TEST(Long, long, long)
ARRAY_TEST(Int, int, int)
ARRAY_TEST(Short, short, short)

ARRAY_TEST(ULongLong, ullong, unsigned long long)
ARRAY_TEST(ULong, ulong, unsigned long)
ARRAY_TEST(UInt, uint, unsigned int)
ARRAY_TEST(UShort, ushort, unsigned short)

ARRAY_TEST(Char, char, char)
ARRAY_TEST(UChar, uchar, unsigned char)

ARRAY_TEST(Int8, int8,  int8_t)
ARRAY_TEST(Int16, int16, int16_t)
ARRAY_TEST(Int32, int32, int32_t)
ARRAY_TEST(Int64, int64, int64_t)

ARRAY_TEST(UInt8, uint8,  uint8_t)
ARRAY_TEST(UInt16, uint16, uint16_t)
ARRAY_TEST(UInt32, uint32, uint32_t)
ARRAY_TEST(UInt64, uint64, uint64_t)


/** ---------------------------------------------------------------------------
 * struct Obj
 */
struct Obj {
    int i;
    double f;
};

/**
 * Create Obj
 */
struct Obj *create_obj (int i, double f)
{
    struct Obj *obj = (struct Obj *) calloc(1, sizeof(struct Obj));
    core_assert_(obj != NULL, "failed to alloc obj");
    obj->i = i;
    obj->f = f;
    return obj;
}

/**
 * Destroy Obj
 */
void destroy_obj (void *obj)
{
    free(obj);
}


/** ---------------------------------------------------------------------------
 * Array test client
 */
void test_array (void)
{
    test_array_ldouble();
    test_array_double();
    test_array_float();

    test_array_llong();
    test_array_long();
    test_array_int();
    test_array_short();

    test_array_ullong();
    test_array_ulong();
    test_array_uint();
    test_array_ushort();

    test_array_char();
    test_array_uchar();

    test_array_int8();
    test_array_int16();
    test_array_int32();
    test_array_int64();

    test_array_uint8();
    test_array_uint16();
    test_array_uint32();
    test_array_uint64();

    {
        /*
         * Create a array of objects
         */
        struct ArrayVoid *arr = create_array_void();
        core_assert_(array_void_is_empty(arr), "FAIL\n"); \
        core_assert_(arr->size == 0, "FAIL\n"); \
        core_assert_(arr->capacity == 1, "FAIL\n"); \

        /*
         * Insert objects into the array
         */
        const size_t n_objs = 10000000;
        for (size_t i = 0; i < n_objs; ++i) {
            array_void_push(arr, create_obj(i, (double)(2*i)));
            core_assert_(!array_void_is_empty(arr), "FAIL\n"); \
            core_assert_(arr->size == i+1, "FAIL\n"); \
        }

        for (size_t i = 0; i < arr->size; ++i) {
            struct Obj *o = (struct Obj *) arr->data[i];
            core_assert_(o->i == i, "FAIL\n");
            core_assert_(o->f == (double)(2*i), "FAIL\n");
        }

        while (!array_void_is_empty(arr)) {
            destroy_obj(array_void_pop(arr));
        }
    }
}
