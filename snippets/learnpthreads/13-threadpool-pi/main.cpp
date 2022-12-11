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
 * Execute
 */
struct Work {
    uint64_t n_steps;
    double xlo;
    double xhi;
    double integral;
};

void Execute(Work &work)
{
    auto fval = [] (double x) -> double {
        return 4.0 / (1.0 + x*x);
    };

    double dx = (work.xhi - work.xlo)  / static_cast<double>(work.n_steps);
    double fsum = fval(work.xlo) + fval(work.xhi);
    for (size_t k = 1; k < work.n_steps - 1; ++k) {
        double x = work.xlo + static_cast<double>(k) * dx;
        fsum += fval(x);
    }

    work.integral = dx * fsum;
}

static double IntegralPi = 0.0;

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
struct ThreadPool {
    /* Thread Pool state */
    std::queue<Work> work_queue;
    bool queue_is_finished;
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_cond_empty;
    pthread_cond_t queue_cond_not_empty;
    std::vector<pthread_t> work_threads;
    pthread_attr_t thread_attr;

    void Create(const uint64_t n_threads);
    void Destroy(void);

    void Wait(void);
    void Enqueue(const void *arg);

    static void *Thread(void *arg);
};

/**
 * ThreadPool::Create
 * @brief Create a pool with a specified number of work_threads.
 */
void ThreadPool::Create(const uint64_t n_threads)
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
    work_threads.resize(n_threads);
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
    for (auto &thread : work_threads) {
        pthread_create(
            &thread,
            &thread_attr,
            Thread,
            this);
    }
}

/**
 * ThreadPool::Destroy
 * @brief Destroy the thread pool pool and join all threads.
 */
void ThreadPool::Destroy(void)
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
 * ThreadPool::Enqueue
 * @brief Insert a new work item into the work queue and signal the condition
 * queue_cond to awake thread and execute the work.
 */
void ThreadPool::Enqueue(const void *arg)
{
    const Work *work = static_cast<const Work *>(arg);
    pthread_mutex_lock(&queue_lock);
    work_queue.push(*work);
    std::cout << "PoolEnqueue "
        << work->xlo << " "
        << work->xhi << " "
        << work->n_steps << " "
        << ", queue size " << work_queue.size() << "\n";
    pthread_cond_broadcast(&queue_cond_not_empty);
    pthread_mutex_unlock(&queue_lock);
}

/**
 * ThreadPool::Wait
 * @brief Wait blocks until all work items in have been processed, ie while
 * the work queue is not empty.
 */
void ThreadPool::Wait(void)
{
    /*
     * If queue is not empty, sleep and wait for a signal on the condition that
     * queue is empty (sent by a worker thread).
     */
	pthread_mutex_lock(&queue_lock);
    while (!work_queue.empty()) {
        std::cout << "PoolWait sleep"
            << std::boolalpha << " is empty " << work_queue.empty() << "\n";
        pthread_cond_wait(&queue_cond_empty, &queue_lock);
    }
    std::cout << "PoolWait wake up"
        << std::boolalpha << " is empty " << work_queue.empty() << "\n";
    pthread_mutex_unlock(&queue_lock);
}

/**
 * ThreadPool::Thread
 * @brief Execute a work item from the queue.
 */
void *ThreadPool::Thread(void *arg)
{
    ThreadPool *pool = static_cast<ThreadPool *>(arg);

    /** Run while the queue is not empty */
    while (true) {
        /*
         * If the queue is empty, sleep and wait for a signal on the condition
         * queue is not empty (sent by the job enqueue function).
         */
        pthread_mutex_lock(&(pool->queue_lock));
        while (!pool->queue_is_finished && pool->work_queue.empty()) {
            pthread_cond_wait(&(pool->queue_cond_not_empty), &(pool->queue_lock));
        }

        if (pool->queue_is_finished) {
            std::cout << "PoolThread" << " queue_is_finished " << "\n";
            pthread_mutex_unlock(&(pool->queue_lock));
            pthread_exit(NULL);
        }

        /*
         * Pop a work item from the queue. If the queue is empty, wake up
         * wait thread on the condition queue is empty.
         */
        Work work = pool->work_queue.front();
        pool->work_queue.pop();

        std::cout << "PoolThread work item "
            << work.xlo << " "
            << work.xhi << " "
            << work.n_steps << ", queue size "
            << pool->work_queue.size() << "\n";

        if (pool->work_queue.empty()) {
            pthread_cond_signal(&(pool->queue_cond_empty));
        }
        pthread_mutex_unlock(&(pool->queue_lock));

        /* Execute work item and store the result. */
        Execute(work);
        pthread_mutex_lock(&(pool->queue_lock));
        IntegralPi += work.integral;
        std::cout << "PoolThread IntegralPi "
            << " work.integral " << work.integral
            << " IntegralPi " << IntegralPi << "\n";
        pthread_mutex_unlock(&(pool->queue_lock));
    }
}

/** ---------------------------------------------------------------------------
 * main
 */
int main (int argc, char *argv[])
{
    static constexpr uint64_t num_threads = 16;
    static constexpr uint64_t num_work_items = 64;
    static constexpr uint64_t num_work_steps = 1073741824;

    ThreadPool tpool;
    tpool.Create(num_threads);

    double dx = 1.0 / static_cast<double>(num_work_items);
    for (uint64_t k = 0; k < num_work_items; ++k) {
        double xlo = dx * static_cast<double>(k);
        double xhi = dx * static_cast<double>(k+1);
        Work work{num_work_steps, xlo, xhi, 0.0};
        tpool.Enqueue((void *) &work);
    }
    tpool.Wait();
    tpool.Destroy();

    std::cout << "IntegralPi " << std::setprecision(15)
        << IntegralPi << ", "
        << M_PI << " "
        << std::abs(IntegralPi - M_PI)
        << "\n";
    exit(EXIT_SUCCESS);
}

