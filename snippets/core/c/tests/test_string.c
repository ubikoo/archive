/*
 * test_string.c
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 *
 *  See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "test_string.h"

/** ---------------------------------------------------------------------------
 * debug_string
 * @brief Print string debug info
 */
void debug_string (struct String *str)
{
    printf("%p\n", (void *) str);
    printf("size %lu\n", str->size);
    printf("capacity %lu\n", str->capacity);
    printf("data %p\n", (void *) str->data);
    printf("is_empty %s\n", string_is_empty(str) ? "true" : "false");
}

void print_string (struct String *str)
{
    const char *p = str->data;
    printf("[");
    while (*p != '\0') {
        printf("%c", *p++);
    }
    printf("] %lu, %lu\n", str->size, str->capacity);
}


/** ---------------------------------------------------------------------------
 * String test client
 */
void test_string (void)
{
    {
        struct String *str = create_string(NULL);
        print_string(str);
        debug_string(str);
        core_assert_(string_is_empty(str), "FAIL\n");
        core_assert_(str->size == 0, "FAIL\n");
        core_assert_(str->capacity == 1, "FAIL\n");

        /* --------------------------------------------------------------------
         * Push a
         */
        string_push(str, 'a');
        print_string(str);
        debug_string(str);
        core_assert_(!string_is_empty(str), "FAIL\n");
        core_assert_(str->size == 1, "FAIL\n");
        core_assert_(str->capacity == 2, "FAIL\n");

        /* Push b */
        string_push(str, 'b');
        print_string(str);
        debug_string(str);
        core_assert_(!string_is_empty(str), "FAIL\n");
        core_assert_(str->size == 2, "FAIL\n");
        core_assert_(str->capacity == 4, "FAIL\n");

        /* Push c */
        string_push(str, 'c');
        print_string(str);
        debug_string(str);
        core_assert_(!string_is_empty(str), "FAIL\n");
        core_assert_(str->size == 3, "FAIL\n");
        core_assert_(str->capacity == 4, "FAIL\n");

        /* Push d */
        string_push(str, 'd');
        print_string(str);
        debug_string(str);
        core_assert_(!string_is_empty(str), "FAIL\n");
        core_assert_(str->size == 4, "FAIL\n");
        core_assert_(str->capacity == 8, "FAIL\n");

        /* --------------------------------------------------------------------
         * Pop d
         */
        printf("\npop %c\n", string_pop(str));
        print_string(str);
        debug_string(str);
        core_assert_(!string_is_empty(str), "FAIL\n");
        core_assert_(str->size == 3, "FAIL\n");
        core_assert_(str->capacity == 8, "FAIL\n");

        /* Pop c */
        printf("\npop %c\n", string_pop(str));
        print_string(str);
        debug_string(str);
        core_assert_(!string_is_empty(str), "FAIL\n");
        core_assert_(str->size == 2, "FAIL\n");
        core_assert_(str->capacity == 8, "FAIL\n");

        /* Pop b */
        printf("\npop %c\n", string_pop(str));
        print_string(str);
        debug_string(str);
        core_assert_(!string_is_empty(str), "FAIL\n");
        core_assert_(str->size == 1, "FAIL\n");
        core_assert_(str->capacity == 4, "FAIL\n");

        /* Pop a */
        printf("\npop %c\n", string_pop(str));
        print_string(str);
        debug_string(str);
        core_assert_(string_is_empty(str), "FAIL\n");
        core_assert_(str->size == 0, "FAIL\n");
        core_assert_(str->capacity == 2, "FAIL\n");

        destroy_string(str);
    }

    {
        struct String *str_a = create_string(NULL);
        core_assert_(string_is_empty(str_a), "FAIL\n");
        core_assert_(str_a->size == 0, "FAIL\n");
        core_assert_(str_a->capacity == 1, "FAIL\n");

        string_c_copy(str_a, "a");
        core_assert_(!string_is_empty(str_a), "FAIL\n");
        core_assert_(str_a->size == 1, "FAIL\n");
        core_assert_(str_a->capacity == 2, "FAIL\n");

        struct String *str_b = string_clone(str_a);
        core_assert_(!string_is_empty(str_b), "FAIL\n");
        core_assert_(str_b->size == 1, "FAIL\n");
        core_assert_(str_b->capacity == 2, "FAIL\n");

        string_clear(str_b);
        string_push(str_b, 'b');

        print_string(str_a);
        print_string(str_b);

        struct String *str_c = create_string(NULL);
        for (size_t i = 0; i < 10; ++i) {
            string_append(str_c, str_a);
            string_append(str_c, str_b);
            print_string(str_c);
        }
        for (size_t i = 0; i < str_c->size; ++i) {
            if (i%2 == 0) {
                core_assert_(str_c->data[i] == 'a', "FAIL\n");
            } else {
                core_assert_(str_c->data[i] == 'b', "FAIL\n");
            }
        }

        destroy_string(str_a);
        destroy_string(str_b);
        destroy_string(str_c);
    }
}
