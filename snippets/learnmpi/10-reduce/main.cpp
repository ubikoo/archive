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
    static constexpr int n_elems_per_proc = 10000000;
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
        // data.push_back(dist(rng));
        // data.push_back(static_cast<double>(i));
        data.push_back(1.0);
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
    double local_sum = 0.0;
    for (auto &elem : data) {
        local_sum += elem;
    }
    printf("proc %d of %d, local_sum %f\n", proc_id, n_procs, local_sum);


    /* Reduce all local sums into a single global sum on root process. */
    double global_sum = 0.0;
    MPI_Reduce(
        &local_sum,
        &global_sum,
        1,
        MPI_DOUBLE,
        MPI_SUM,
        Params::root_id,
        MPI_COMM_WORLD);
    printf("proc %d of %d, global_sum %f\n", proc_id, n_procs, global_sum);

    /*
     * Finalize MPI context
     */
    MPI_Finalize();

    /* Done */
    exit(EXIT_SUCCESS);
}
