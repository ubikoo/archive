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
#include <unistd.h>
#include <pthread.h>

/** ---------------------------------------------------------------------------
 * global data
 */
static constexpr size_t k_count_step = 100;
static constexpr size_t k_count_threshold = 10;

struct Data {
    size_t count = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  condition = PTHREAD_COND_INITIALIZER;
} g_data;

/** ---------------------------------------------------------------------------
 * work_count
 * Increment the global counter. When the threshold value condition is reached
 * signal the waiting thread to wake up. This occurs while the mutex is locked.
 */
void *work_count(void *ptr)
{
    Data *o = (Data *) ptr;
    while (true) {
        pthread_mutex_lock(&o->mutex);

        o->count++;
        if (o->count == k_count_threshold) {
            std::cout << "work_count: threshold reached " << o->count << std::endl;
            pthread_cond_signal(&o->condition);
            std::cout << "work_count: sent signal " << o->count << std::endl;
        }

        pthread_mutex_unlock(&o->mutex);

        if (o->count >= k_count_threshold) {
            pthread_exit(NULL);
        }
    }
}

/** ---------------------------------------------------------------------------
 * watch_count
 * Lock mutex and wait for signal. Note that the pthread_cond_wait routine
 * will automatically and atomically unlock mutex while it waits.
 * Also, note that if COUNT_LIMIT is reached before this routine is run by
 * the waiting thread, the loop will be skipped to prevent pthread_cond_wait
 * from never returning.
 */
void *watch_count(void *ptr)
{
    Data *o = (Data *) ptr;

    pthread_mutex_lock(&o->mutex);

    while (o->count < k_count_threshold) {
        std::cout << "watch_count: going to wait " << o->count << std::endl;
        pthread_cond_wait(&o->condition, &o->mutex);
        std::cout << "watch_count: going to wake up " << o->count << std::endl;
    }

    o->count += k_count_step;
    std::cout << "watch_count: update count to " << o->count << std::endl;

    pthread_mutex_unlock(&o->mutex);

    pthread_exit(NULL);
}

/** ---------------------------------------------------------------------------
 * main
 */
int main (int argc, char *argv[])
{
    /* For portability, explicitly create threads in a joinable state */
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

    /* Execute threads */
    std::array<pthread_t, 2> threads;
    pthread_create(&threads[0], &thread_attr, work_count, (void *) &g_data);
    pthread_create(&threads[1], &thread_attr, watch_count, (void *) &g_data);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    std::cout << "final count " << g_data.count << std::endl;

    /* Clean up and exit */
    pthread_attr_destroy(&thread_attr);

    exit(EXIT_SUCCESS);
}
