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

/**
 * @brief Master Id
 */
static const int kMasterId = 0;

/**
 * main
 */
int main (int argc, char *argv[])
{
    /* Initialize MPI context */
    MPI_Init(&argc, &argv);

    /* Get MPI world size and rank */
    int n_tasks;
    MPI_Comm_size(MPI_COMM_WORLD, &n_tasks);
    int task_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &task_id);

    /* Do MPI work */
    {
        int token = 0;
        const int src_id = 0;
        const int dst_id = 1;

        if (task_id == src_id) {
            token = INT_MAX;
            MPI_Send((void *)&token, 1, MPI_INT, dst_id, 0, MPI_COMM_WORLD);
        }

        if (task_id == dst_id) {
            MPI_Recv((void *)&token, 1, MPI_INT, src_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        printf("Task %d of %d, token %d\n", task_id, n_tasks, token);
    }

    /* Finalize MPI context */
    MPI_Finalize();

    /* Done */
    exit(EXIT_SUCCESS);
}
