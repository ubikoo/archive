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

int r3 = 0;
pthread_mutex_t r3_mutex = PTHREAD_MUTEX_INITIALIZER;

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
        printf("do_thing_a value %d\n", *value);
        double x = 0;
        for (size_t j = 0; j < max_iters; ++j) { x += delta_x; }
    }

    /* Mutex update of r3 value */
    pthread_mutex_lock(&r3_mutex);
    for (size_t i = 0; i < 4; ++i) {
        r3++;
        printf("do_thing_a r3 %d\n", r3);
    }
    pthread_mutex_unlock(&r3_mutex);

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

    /* Mutex update of r3 value */
    pthread_mutex_lock(&r3_mutex);
    for (size_t i = 0; i < 4; ++i) {
        r3++;
        printf("do_thing_b r3 %d\n", r3);
    }
    pthread_mutex_unlock(&r3_mutex);

    pthread_exit(NULL);
}

/** ---------------------------------------------------------------------------
 * do_wrap_up
 */
void do_wrap_up(int a, int b, int c)
{
    printf("do_wrap_up, a %d, b %d, c %d\n", a, b, c);
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

    do_wrap_up(r1, r2, r3);

    exit(EXIT_SUCCESS);
}
