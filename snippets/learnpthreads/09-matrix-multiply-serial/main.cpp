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
    static constexpr size_t k_dim = 1024;
    std::vector<float> A;
    std::vector<float> B;
    std::vector<float> C;
} g_data;


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

void Execute(void)
{
    size_t k_dim = Data::k_dim;
    std::vector<float> &A = g_data.A;
    std::vector<float> &B = g_data.B;
    std::vector<float> &C = g_data.C;

    for (size_t i = 0; i < k_dim; ++i) {
        for (size_t j = 0; j < k_dim; ++j) {
            float sum  = 0.0f;
            for (size_t k = 0; k < k_dim; ++k) {
                sum += A[i * k_dim + k] * B[k * k_dim + j];
            }
            C[i * k_dim + j] = sum;
        }
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
