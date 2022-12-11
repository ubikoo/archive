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

    /* Get MPI world size */
    int n_tasks;
    MPI_Comm_size(MPI_COMM_WORLD, &n_tasks);

    /* Get my rank */
    int task_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &task_id);

    /* Get the hostname */
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(hostname, &name_len);

    /* Do MPI work */
    {
        if (kMasterId == task_id) {
           printf("Hostname: %s\n", hostname);
        }
        printf("Task %d of %d\n", task_id, n_tasks);
    }

    /* Finalize MPI context */
    MPI_Finalize();

    /* Done */
    exit(EXIT_SUCCESS);
}
