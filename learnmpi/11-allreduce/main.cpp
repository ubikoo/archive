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
#include <mpi.h>

/** ---------------------------------------------------------------------------
 * @brief Parameter constants
 */
namespace Params {
    static constexpr int root_id = 0;
    static constexpr int n_elems_per_proc = 100000000;
} /* namespace Params */

/** ---------------------------------------------------------------------------
 * @brief Create random data.
 */
std::vector<double> CreateData(int proc_id, int n_procs, int n_elems_per_proc)
{
    std::random_device seed;    /* rng device */
    std::mt19937 rng(seed());   /* rng engine */
    std::uniform_real_distribution<double> dist(0.0f, 1.0f);

    std::vector<double> data;
    for (int i = 0; i < n_elems_per_proc; ++i) {
        data.push_back(dist(rng));
        // data.push_back(static_cast<double>(i));
        // data.push_back(1.0);
    }

    return data;
}

/** ---------------------------------------------------------------------------
 * main
 */
int main (int argc, char *argv[])
{
    /*
     * Initialize MPI context
     */
    MPI_Init(&argc, &argv);

    /* Get MPI world size and rank */
    int n_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    int proc_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    /* Create an array of random data and corresponding sum. */
    std::vector<double> data = CreateData(proc_id, n_procs, Params::n_elems_per_proc);

    /* Reduce all local sums into a single global sum on root process. */
    double local_sum = 0.0;
    for (auto &elem : data) {
        local_sum += elem;
    }

    double global_sum = 0.0;
    MPI_Allreduce(
        &local_sum,
        &global_sum,
        1,
        MPI_DOUBLE,
        MPI_SUM,
        MPI_COMM_WORLD);
    printf("proc %d of %d, local_sum %lf global_sum %lf \n",
        proc_id, n_procs, local_sum, global_sum);

    /* Compute local variance and send it to root process */
    double global_avg = global_sum / static_cast<double>(n_procs * data.size());

    double local_var = 0.0;
    for (auto &elem : data) {
        double var = elem - global_avg;
        local_var += var * var;
    }

    double global_var = 0.0;
    MPI_Reduce(
        &local_var,
        &global_var,
        1,
        MPI_DOUBLE,
        MPI_SUM,
        Params::root_id,
        MPI_COMM_WORLD);
    global_var /= static_cast<double>(n_procs * data.size());

    if (proc_id == Params::root_id) {
        printf("mean %lf, sdev %lf:\n", global_avg, std::sqrt(global_var));
    }

    /*
     * Finalize MPI context
     */
    MPI_Finalize();

    /* Done */
    exit(EXIT_SUCCESS);
}
