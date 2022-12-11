/*
 * main.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include <memory>
#include <chrono>
#include <numeric>
#include <vector>

#include "atto/opencl/opencl.hpp"
#include "model.hpp"
#include "threadpool.hpp"
using namespace atto;

/**
 * main test client
 */
int main(int argc, char const *argv[])
{
    /* Read arguments */
    core_assert(argc == 5, "insufficient arguments");
    uint64_t n_threads = std::stoi(argv[1]);
    uint64_t n_items = std::stoi(argv[2]);
    uint64_t n_iters = std::stoi(argv[3]);
    double p_site = std::stod(argv[4]);

    uint64_t n_models = n_threads * n_items;
    uint64_t n_samples = n_models * n_iters;

    std::cout << argv[0]
        << " n_threads " << n_threads
        << " n_items " << n_items
        << " n_iters " << n_iters
        << " p_site " << p_site
        << "\n";

    /* Create the thread pool */
    ThreadPool threadpool;
    threadpool.create(n_threads);

    /* Execute a percolation model on each thread */
    std::array<uint64_t, 3> percolate = {0, 0, 0};
    std::vector<Model> model(n_models);
    for (size_t iter = 0; iter < n_iters; ++iter) {
        for (size_t ix = 0; ix < n_models; ++ix) {
            ThreadWork work{p_site, &model[ix], &percolate};
            threadpool.enqueue((void *) &work);
        }
        threadpool.wait();

        /* DEBUG */
        if (iter % 16 == 0)
        {
            double ns = static_cast<double>((iter + 1) * n_models);
            double px = static_cast<double>(percolate[0]) / ns;
            double py = static_cast<double>(percolate[1]) / ns;
            double pb = static_cast<double>(percolate[2]) / ns;
            std::cout << "percolate " << px << " " << py << " " << pb << "\n";

        }
    }
    threadpool.destroy();

    exit(EXIT_SUCCESS);
}
