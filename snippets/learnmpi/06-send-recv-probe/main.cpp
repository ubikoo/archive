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
        const int kTag = 0;

        if (task_id == 0) {
            /* Pick a random amount of integers to send to process 1 */
            const int kMaxNumbers = 100;
            std::random_device seed;    /* rng device */
            std::mt19937 rng(seed());   /* rng engine */
            std::uniform_int_distribution<int> dist(0, kMaxNumbers);
            int n_count = dist(rng);

            /* Initialize numbers */
            static std::vector<int> numbers(kMaxNumbers);
            for (int i = 0; i < kMaxNumbers; ++i) {
                numbers[i] = i < n_count ? i : -1;
            }

            /* Send the count integers to process 1 */
            MPI_Send(
                (void *)&numbers[0],
                n_count,
                MPI_INT,
                1,
                kTag,
                MPI_COMM_WORLD);
            printf("0 sent %d numbers to 1\n", n_count);
        } else if (task_id == 1) {
            /*
             * Probe the sender to retrieve the receive status. The status object
             * contains the size and other attributes of the incoming message.
             * Retrieve the message size.
             */
            MPI_Status status;
            MPI_Probe(0, kTag, MPI_COMM_WORLD, &status);

            int n_count;
            MPI_Get_count(&status, MPI_INT, &n_count);
            printf("Get count from status %d\n", n_count);

            /* Allocate buffer storage to receive the numbers */
            std::vector<int> numbers(n_count);
            MPI_Recv(
                (void *)&numbers[0],
                (int) numbers.size(),
                 MPI_INT,
                 0,
                 kTag,
                 MPI_COMM_WORLD,
                 &status);

            /* Print the received status information. */
            printf("1 dynamically received %d numbers from 0.\n", n_count);
            for (int i = 0; i < n_count; ++i) {
                printf("i = %d, numbers = %d\n", i, numbers[i]);
            }
        }
    }

    /* Finalize MPI context */
    MPI_Finalize();

    /* Done */
    exit(EXIT_SUCCESS);
}
