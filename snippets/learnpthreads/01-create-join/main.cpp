/*
 * main.c++
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include <bits/stdc++.h>
#include <pthread.h>

static const size_t max_iters = 1000000000;
int r1 = 0;
int r2 = 0;

/** ---------------------------------------------------------------------------
 * do_thing_a
 */
void *do_thing_a(void *ptr)
{
    static const double delta_x = 1.0 / static_cast<double>(max_iters);

    /* Update argument */
    int *value = (int *) ptr;
    for (size_t i = 0; i < 4; ++i) {
        (*value)++;
        printf("do_thing_a %d\n", *value);
        double x = 0;
        for (size_t j = 0; j < max_iters; ++j) { x += delta_x; }
    }
    pthread_exit(NULL);
}

/** ---------------------------------------------------------------------------
 * do_thing_b
 */
void *do_thing_b(void *ptr)
{
    static const double delta_x = 1.0 / static_cast<double>(max_iters);

    /* Update argument */
    int *value = (int *) ptr;
    for (size_t i = 0; i < 4; ++i) {
        (*value)++;
        printf("do_thing_b value %d\n", *value);
        double x = 0;
        for (size_t j = 0; j < max_iters; ++j) { x += delta_x; }
    }
    pthread_exit(NULL);
}

/** ---------------------------------------------------------------------------
 * do_wrap_up
 */
void do_wrap_up(int a, int b)
{
    printf("do_wrap_up, do_thing_a %d, do_thing_b %d\n", a, b);
}

/** ---------------------------------------------------------------------------
 * main
 */
int main (int argc, char *argv[])
{
    /* Run thread a */
    pthread_t thread_a;
    pthread_create(&thread_a, NULL, do_thing_a, (void *) &r1);

    /* Run thread b */
    pthread_t thread_b;
    pthread_create(&thread_b, NULL, do_thing_b, (void *) &r2);

    /* Join threads again */
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    do_wrap_up(r1, r2);

    exit(EXIT_SUCCESS);
}
