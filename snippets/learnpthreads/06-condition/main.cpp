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

static constexpr size_t n_threads = 3;
static constexpr size_t n_iterations = 10;
static constexpr size_t count_threshold = 8;

size_t count = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  count_condition = PTHREAD_COND_INITIALIZER;

/** ---------------------------------------------------------------------------
 * inc_count
 */
void *inc_count(void *ptr)
{
    for (size_t i = 0; i < n_iterations; i++) {
        pthread_mutex_lock(&count_mutex);
        /*
         * Increment the global counter. When its condition value is reached,
         * signal the waiting thread. This occurs while the mutex is locked.
         */
        count++;
        if (count == count_threshold) {
            std::cout << "inc_count: " << count << " threshold reached" << std::endl;
            pthread_cond_signal(&count_condition);
            std::cout << "inc_count: send signal" << count << std::endl;
        }
        pthread_mutex_unlock(&count_mutex);

        /* Do some work so threads can alternate on mutex lock */
        usleep(100000);
    }

    pthread_exit(NULL);
}

/** ---------------------------------------------------------------------------
 * watch_count
 */
void *watch_count(void *ptr)
{
    /*
     * Lock mutex and wait for signal. Note that the pthread_cond_wait routine
     * will automatically and atomically unlock mutex while it waits.
     * Also, note that if COUNT_LIMIT is reached before this routine is run by
     * the waiting thread, the loop will be skipped to prevent pthread_cond_wait
     * from never returning.
     */
    pthread_mutex_lock(&count_mutex);
    while (count < count_threshold) {
        std::cout << "watch_count: count = " << count << ", going to wait" << std::endl;
        pthread_cond_wait(&count_condition, &count_mutex);
        std::cout << "watch_count: count = " << count << ", wake up signal" << std::endl;
    }
    count += 125;
    std::cout << "watch_count: updating the value of count to " << count << std::endl;
    pthread_mutex_unlock(&count_mutex);
    pthread_exit(NULL);
}

/** ---------------------------------------------------------------------------
 * main
 */
int main (int argc, char *argv[])
{
    /* For portability, explicitly create threads in a joinable state */
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* Execute threads */
    std::array<pthread_t, n_threads> threads;
    pthread_create(&threads[0], &attr, watch_count, NULL);
    pthread_create(&threads[1], &attr, inc_count, NULL);
    pthread_create(&threads[2], &attr, inc_count, NULL);

    /* Wait for all threads to complete */
    for (size_t i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    std::cout << "final count " << count << std::endl;

    /* Clean up and exit */
    pthread_attr_destroy(&attr);

    exit(EXIT_SUCCESS);

}
