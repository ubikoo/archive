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
    std::vector<double> data;
    if (proc_id > Params::root_id) {
        return data;
    }

    std::random_device seed;    /* rng device */
    std::mt19937 rng(seed());   /* rng engine */
    std::uniform_real_distribution<double> dist(0.0f, 1.0f);

    for (int i = 0; i < n_elems_per_proc * n_procs; ++i) {
        data.push_back(dist(rng));
        // data.push_back(static_cast<double>(i));
    }

    double data_sum = 0.0;
    for (auto &elem : data) {
        data_sum += elem;
    }
    printf("CreateData: proc %d of %d, data_sum %f\n", proc_id, n_procs, data_sum);

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

    /* Create an array of random data. */
    std::vector<double> data = CreateData(proc_id, n_procs, Params::n_elems_per_proc);

    /* Scatter the random data to all processes. */
    std::vector<double> subdata(Params::n_elems_per_proc);
    MPI_Scatter(
        (void *) &data[0],
        subdata.size() * sizeof(double),
        MPI_BYTE,
        (void *) &subdata[0],
        subdata.size() * sizeof(double),
        MPI_BYTE,
        Params::root_id,
        MPI_COMM_WORLD);

    /* Compute the sum of all the elements in the subdata buffer. */
    double subdata_sum = 0.0;
    for (auto &elem : subdata) {
        subdata_sum += elem;
    }

    std::vector<double> all_subdata_sum(n_procs, 0.0f);
    MPI_Allgather(
        (void *) &subdata_sum,
        sizeof(double),
        MPI_BYTE,
        (void *) &all_subdata_sum[0],
        sizeof(double),
        MPI_BYTE,
        MPI_COMM_WORLD);

    {
        double sum = 0.0f;
        for (auto &it : all_subdata_sum) {
            sum += it;
        }
        printf("proc_id %d: all_subdata_sum %f\n", proc_id, sum);
    }

    /*
     * Finalize MPI context
     */
    MPI_Finalize();

    /* Done */
    exit(EXIT_SUCCESS);
}
