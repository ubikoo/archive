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

static constexpr size_t count_done = 10;
static constexpr size_t count_halt_beg = 4;
static constexpr size_t count_halt_end = 8;
size_t count = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;

/** ---------------------------------------------------------------------------
 * execute_a
 *
 * Do work until a certain condition must occur, in this case, count is in the interval:
 *  count_halt_beg <= count <= count_halt_end
 *
 * Lock the associated condition mutex and check the value of the global variable.
 * If the condition is met, call the pthread_cond_wait to perform a blocking wait for signal from
 * execute_b.
 *
 * The pthread_cond_wait call automatically unlocks the associated mutex variable so that it can be
 * used by thread execute_b. When execute_b send send signal, execute_a wakes up. The mutex is
 * automatically and atomically locked again.
 * Explicitly unlock the mutex.
 */
void *execute_a(void *ptr)
{
    while (true) {
        /* Wait on the condition */
        pthread_mutex_lock(&condition_mutex);
        while (count >= count_halt_beg && count <= count_halt_end) {
            std::cout << "execute_a wait begin: " << count << std::endl;
            pthread_cond_wait(&condition, &condition_mutex);
            std::cout << "execute_a wait end: " << count << std::endl;
        }
        pthread_mutex_unlock(&condition_mutex);

        /* Update global variable */
        pthread_mutex_lock(&count_mutex);
        count++;
        std::cout << "execute_a: count " << count << std::endl;
        pthread_mutex_unlock(&count_mutex);

        if (count >= count_done) {
            pthread_exit(NULL);
        }
    }
}

/** ---------------------------------------------------------------------------
 * execute_b
 *
 * Do work:
 *  - lock the associated mutex with the global variable
 *  - change the global variable execute_a is waiting upon.
 *  - check the value of the global count variable. if it fulfills the specified condition, signal
 *    execute_a to wake up
 *  - unlock the associated mutex with the global variable
 */
void *execute_b(void *ptr)
{
    while (true) {
        /* Signal the condition */
        pthread_mutex_lock(&condition_mutex);
        if (count < count_halt_beg || count > count_halt_end) {
            std::cout << "execute_b signal begin: " << count << std::endl;
            pthread_cond_signal(&condition);
            std::cout << "execute_b signal end: " << count << std::endl;
        }
        pthread_mutex_unlock(&condition_mutex);

        /* Update the global variable */
        pthread_mutex_lock(&count_mutex);
        count++;
        std::cout << "execute_b: count " << count << std::endl;
        pthread_mutex_unlock(&count_mutex);

        if (count >= count_done) {
            pthread_exit(NULL);
        }
    }
}

/** ---------------------------------------------------------------------------
 * main
 */
int main (int argc, char *argv[])
{
    /* Execute threads */
    pthread_t thread_a, thread_b;

    pthread_create(&thread_a, NULL, &execute_a, NULL);
    pthread_create(&thread_b, NULL, &execute_b, NULL);

    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    std::cout << "final count " << count << std::endl;

    exit(EXIT_SUCCESS);
}
