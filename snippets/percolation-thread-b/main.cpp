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
    core_assert(argc == 2, "insufficient arguments");
    double p_site = std::stod(argv[1]);

    /* Create the thread pool */
    ThreadPool threadpool;
    threadpool.create(Params::n_threads);

    /* Execute a percolation model on each thread */
    std::vector<Model> model(Params::n_models);
    std::array<uint64_t, 3> percolate = {0, 0, 0};
    uint64_t n_samples = 0;
    uint64_t n_freq = Params::n_iters / 8;
    for (size_t iter = 0; iter < Params::n_iters; ++iter) {
        /* Compute models */
        for (size_t ix = 0; ix < Params::n_models; ++ix) {
            n_samples++;
            ThreadWork work{p_site, &model[ix], &percolate};
            threadpool.enqueue((void *) &work);
        }
        threadpool.wait();

        /* Compute statistics */
        n_samples += Params::n_models;

        /* DEBUG */
        if (iter % n_freq == 0)
        {
            double ns = static_cast<double>(n_samples);
            double px = static_cast<double>(percolate[0]) / ns;
            double py = static_cast<double>(percolate[1]) / ns;
            double pb = static_cast<double>(percolate[2]) / ns;
            std::cout
                << " p_site " << p_site
                << " percolate " << px << " " << py << " " << pb << "\n";

        }
    }
    threadpool.destroy();

    exit(EXIT_SUCCESS);
}
