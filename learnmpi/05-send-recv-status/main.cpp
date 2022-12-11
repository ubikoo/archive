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
        /* Initialize numbers */
        const int MAX_NUMBERS = 100;
        int numbers[MAX_NUMBERS];
        for (size_t i = 0; i < MAX_NUMBERS; ++i) {
            numbers[i] = -1;
        }

        int n_count;
        if (task_id == 0) {
            /* Set numbers */
            const int MAX_NUMBERS = 100;
            int numbers[MAX_NUMBERS];
            for (size_t i = 0; i < MAX_NUMBERS; ++i) {
                numbers[i] = i;
            }

            /* Pick a random amount of integers to send to process 1 */
#if 0
            srand(time(NULL));
            n_count = (rand() / (float) RAND_MAX) * MAX_NUMBERS;
#else
            std::random_device seed;    /* rng device */
            std::mt19937 rng(seed());   /* rng engine */
            std::uniform_int_distribution<int> dist(0, MAX_NUMBERS);
            n_count = dist(rng);
#endif

            /* Send the count integers to process 1 */
            MPI_Send(numbers, n_count, MPI_INT, 1, 0, MPI_COMM_WORLD);
            printf("0 sent %d numbers to 1\n", n_count);
        } else if (task_id == 1) {
            /* Receive at most MAX_NUMBERS from process 0 and store the */
            MPI_Status status;
            MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

            /*
             * After receiving the message, check the receive status to
             * determine how many numbers were actually received.
             */
            MPI_Get_count(&status, MPI_INT, &n_count);

            /*
             * Print the received count numbers, and receive status information.
             */
            printf("1 received %d numbers from 0. Message source = %d, tag = %d\n",
                n_count, status.MPI_SOURCE, status.MPI_TAG);

            for (size_t i = 0; i < MAX_NUMBERS; ++i) {
                printf("i = %ld, numbers = %d\n", i, numbers[i]);
            }
        }
    }

    /* Finalize MPI context */
    MPI_Finalize();

    /* Done */
    exit(EXIT_SUCCESS);
}
