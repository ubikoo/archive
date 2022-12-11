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

int counter = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
static constexpr uint32_t n_threads = 16;
static constexpr size_t n_iters = 1000000000;

/** ---------------------------------------------------------------------------
 * execute
 */
void *execute(void *ptr)
{
    size_t x = 0;
    for (size_t j = 0; j < n_iters; ++j) { x += 1; }

    pthread_mutex_lock(&counter_mutex);
    counter++;
    std::cout << "Thread execute " << counter << "\n";
    pthread_mutex_unlock(&counter_mutex);
    pthread_exit(NULL);
}

/** ---------------------------------------------------------------------------
 * main
 */
int main (int argc, char *argv[])
{
    /* Execute threads */
    std::array<pthread_t, n_threads> threads;
    for (size_t i = 0; i < threads.size(); ++i) {
        pthread_create(&threads[i], NULL, execute, NULL);
    }

    /* Execute threads */
    for (size_t i = 0; i < threads.size(); ++i) {
        pthread_join(threads[i], NULL);
    }

    std::cout << "Final counter " << counter << "\n";

    exit(EXIT_SUCCESS);
}
