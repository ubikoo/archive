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
 * Thread pool
 * The thread pool key data structure is a queue.
 * When a worker looks at the queue and finds it empty, it sleeps on the
 * queue_cond condition.
 *
 * When a new work item is added to the queue, a worker is awaken with a
 * signal on the queue_cond condition.
 *
 * After the worker finishes the job, broadcast a signal to all threads.
 * If it is a worker thread, pull the next job. If it is the waiter thread,
 * check if the queue is empty.
 *
 * https://stackoverflow.com/questions/6954489/how-to-utilize-a-thread-pool-with-pthreads
 */
void PoolCreate(void);
void PoolDestroy(void);
void PoolEnqueue(const void *arg);
void PoolWait(void);
void *PoolThread(void *arg);

static constexpr size_t num_work_items = 512;
static constexpr size_t num_threads = 16;
static constexpr size_t num_iters = 8192;

typedef uint32_t Work;
std::queue<Work> work_queue;
bool queue_is_finished;

pthread_mutex_t queue_lock;
pthread_cond_t queue_cond_empty;
pthread_cond_t queue_cond_not_empty;

std::vector<pthread_t> work_threads;
pthread_attr_t work_thread_attr;

/**
 * PoolCreate
 * @brief Create the thread pool with a specified number of threads.
 */
void PoolCreate(void)
{
    /* Initialize queue */
    while (!work_queue.empty()) {
        work_queue.pop();
    }
    queue_is_finished = false;

    /* Initialize the queue mutex and condition variable */
    pthread_mutex_init(&queue_lock, NULL);
    pthread_cond_init(&queue_cond_empty, NULL);
    pthread_cond_init(&queue_cond_not_empty, NULL);

    /* Create a pool of threads */
    work_threads.resize(num_threads);
    pthread_attr_init(&work_thread_attr);
    pthread_attr_setdetachstate(&work_thread_attr, PTHREAD_CREATE_JOINABLE);
    for (auto &thread : work_threads) {
        pthread_create(
            &thread,
            &work_thread_attr,
            PoolThread,
            NULL);
    }
}

/**
 * PoolDestroy
 * @brief Destroy the thread pool pool and join all threads.
 */
void PoolDestroy(void)
{
    /* Set queue finished flag */
    pthread_mutex_lock(&queue_lock);
    queue_is_finished = true;
    pthread_mutex_unlock(&queue_lock);

    /* Wake up any workers so they can recheck the finished flag and exit */
    pthread_cond_broadcast(&queue_cond_not_empty);

    /* Wait for all threads to terminate.  */
    for (auto &thread : work_threads) {
        pthread_join(thread, NULL);
    }
}

/**
 * PoolEnqueue
 * @brief Insert a new work item into the work queue and signal the condition
 * queue_cond to awake thread and execute the work.
 */
void PoolEnqueue(const void *arg)
{
    const Work *work = static_cast<const Work *>(arg);
    pthread_mutex_lock(&queue_lock);
    work_queue.push(*work);
    std::cout << "PoolEnqueue " << *work << ", queue size " << work_queue.size() << "\n";
    pthread_cond_broadcast(&queue_cond_not_empty);
    pthread_mutex_unlock(&queue_lock);
}

/**
 * PoolWait
 * @brief Wait blocks until all work items in have been processed, ie while
 * the work queue is not empty.
 */
void PoolWait(void)
{
    /*
     * If queue is not empty, sleep and wait for a signal on the condition that
     * queue is empty (sent by a worker thread).
     */
	pthread_mutex_lock(&queue_lock);
    while (!work_queue.empty()) {
        std::cout << "PoolWait sleep" << std::boolalpha << " is empty " << work_queue.empty() << "\n";
        pthread_cond_wait(&queue_cond_empty, &queue_lock);
    }
    std::cout << "PoolWait wake up" << std::boolalpha << " is empty " << work_queue.empty() << "\n";
    pthread_mutex_unlock(&queue_lock);
}

/**
 * PoolThread
 * @brief Execute a work item from the queue.
 */
void *PoolThread(void *arg)
{
    /** Dummy execute function */
    auto execute = [&] (const Work &work) -> void {
        size_t x = 0;
        for (size_t i1 = 0; i1 < num_iters; ++i1) {
        for (size_t i2 = 0; i2 < num_iters; ++i2) {
            x++;
        }}
    };

    /** Run while the queue is not empty */
    while (true) {
        /*
         * If the queue is empty, sleep and wait for a signal on the condition
         * queue is not empty (sent by the job enqueue function).
         */
        Work work;

        pthread_mutex_lock(&queue_lock);
        while (!queue_is_finished && work_queue.empty()) {
            pthread_cond_wait(&queue_cond_not_empty, &queue_lock);
        }

        if (queue_is_finished) {
            std::cout << "PoolThread" << " queue_is_finished " << "\n";
            pthread_mutex_unlock(&queue_lock);
            pthread_exit(NULL);
        }

        /*
         * Pop a work item from the queue. If the queue is empty, wake up
         * wait thread on the condition queue is empty.
         */
        work = work_queue.front();
        work_queue.pop();
        std::cout << "PoolThread execute " << " work " << work << ", queue size " << work_queue.size() << "\n";

        if (work_queue.empty()) {
            pthread_cond_signal(&queue_cond_empty);
        }
        pthread_mutex_unlock(&queue_lock);

        /* Execute work item. */
        execute(work);
    }
}

/** ---------------------------------------------------------------------------
 * main
 */
int main (int argc, char *argv[])
{
    PoolCreate();
    for (size_t i = 0; i < num_work_items; ++i) {
        Work work = i;
        PoolEnqueue((const void *) &work);
    }
    PoolWait();
    PoolDestroy();

    exit(EXIT_SUCCESS);
}
