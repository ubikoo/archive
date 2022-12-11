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

struct Data {
    const size_t iters = 1000000000;
    const double delta_x = 1.0 / static_cast<double>(iters);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int r1 = 0;
    int r2 = 0;
    int r3 = 0;
} data;

/** ---------------------------------------------------------------------------
 * do_thing_a
 */
void *do_thing_a(void *ptr)
{
    Data *data_ptr = (Data *) ptr;

    /* Update argument */
    for (size_t i = 0; i < 4; ++i) {
        data_ptr->r1++;
        printf("do_thing_a r1 %d\n", data_ptr->r1);
        double x = 0;
        for (size_t j = 0; j < data_ptr->iters; ++j) { x += data_ptr->delta_x; }
    }

    /* Mutex update of r3 value */
    pthread_mutex_lock(&data_ptr->mutex);
    for (size_t i = 0; i < 8; ++i) {
        data_ptr->r3++;
        printf("do_thing_a r3 %d\n", data_ptr->r3);
    }
    pthread_mutex_unlock(&data_ptr->mutex);

    pthread_exit(NULL);
}

/** ---------------------------------------------------------------------------
 * do_thing_b
 */
void *do_thing_b(void *ptr)
{
    Data *data_ptr = (Data *) ptr;

    /* Update argument */
    for (size_t i = 0; i < 4; ++i) {
        data_ptr->r2++;
        printf("do_thing_b r2 %d\n", data_ptr->r2);
        double x = 0;
        for (size_t j = 0; j < data_ptr->iters; ++j) { x += data_ptr->delta_x; }
    }

    /* Mutex update of r3 value */
    pthread_mutex_lock(&data_ptr->mutex);
    for (size_t i = 0; i < 4; ++i) {
        data_ptr->r3++;
        printf("do_thing_b r3 %d\n", data_ptr->r3);
    }
    pthread_mutex_unlock(&data_ptr->mutex);

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
    pthread_create(&thread_a, NULL, do_thing_a, (void *) &data);

    /* Run thread b */
    pthread_t thread_b;
    pthread_create(&thread_b, NULL, do_thing_b, (void *) &data);

    /* Join threads again */
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    do_wrap_up(data.r1, data.r2, data.r3);

    exit(EXIT_SUCCESS);
}
