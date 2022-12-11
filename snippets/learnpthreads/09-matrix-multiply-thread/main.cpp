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
struct Data {
    static constexpr size_t k_threads = 16;
    static constexpr size_t k_dim = 1024;
    std::vector<float> A;
    std::vector<float> B;
    std::vector<float> C;
} g_data;

struct Item {
    size_t beg;
    size_t end;
    Data *data;
};

/** ---------------------------------------------------------------------------
 * Model setup, execute and teardown functions.
 */
void Setup()
{
    size_t k_dim = Data::k_dim;
    std::vector<float> &A = g_data.A;
    std::vector<float> &B = g_data.B;
    std::vector<float> &C = g_data.C;

    /* Resize matrix dimensions */
    A.resize(k_dim * k_dim);
    B.resize(k_dim * k_dim);
    C.resize(k_dim * k_dim);

    /* Initialize matrix A */
    std::random_device seed;    /* rng device */
    std::mt19937 rng(seed());   /* rng engine */
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (size_t i = 0; i < k_dim; ++i) {
        for (size_t j = 0; j < k_dim; ++j) {
            A[i * k_dim + j] = dist(rng);
        }
    }

    /* B is the transpose of A */
    for (size_t i = 0; i < k_dim; ++i) {
        for (size_t j = 0; j < k_dim; ++j) {
            B[j * k_dim + i] = A[i * k_dim + j] = dist(rng);
        }
    }
}

void *ExecuteItem(void *ptr)
{
    Item *item = (Item *) ptr;
    size_t k_dim = Data::k_dim;
    std::vector<float> &A = item->data->A;
    std::vector<float> &B = item->data->B;
    std::vector<float> &C = item->data->C;

    for (size_t i = item->beg; i < item->end; ++i) {
        for (size_t j = 0; j < k_dim; ++j) {
            float sum  = 0.0f;
            for (size_t k = 0; k < k_dim; ++k) {
                sum += A[i * k_dim + k] * B[k * k_dim + j];
            }
            C[i * k_dim + j] = sum;
        }
    }

    pthread_exit(NULL);
}

void Execute(void)
{
    /* For portability, explicitly create threads in a joinable state */
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

    /* Create a collection of workitems for each thread */
    size_t k_dim = Data::k_dim;
    size_t range = Data::k_dim / Data::k_threads;
    std::vector<Item> items;
    size_t beg = 0;
    while (beg < k_dim) {
        size_t end = beg + range;
        end = (end < k_dim) ? end : k_dim;
        items.push_back({beg, end, &g_data});
        beg += range;
    }

    for (auto &it : items) {
        std::cout << "item " << it.beg << " " << it.end << "\n";
    }

    /* Execute threads */
    std::vector<pthread_t> threads;
    for (auto &it : items) {
        threads.emplace_back();
        pthread_create(&threads.back(), &thread_attr, ExecuteItem, (void *) &it);
    }

    for (auto &it : threads) {
        pthread_join(it, NULL);
    }
}

void Teardown()
{}

/** ---------------------------------------------------------------------------
 * main
 */
int main (int argc, char *argv[])
{
    Setup();
    Execute();
    Teardown();
    exit(EXIT_SUCCESS);
}
