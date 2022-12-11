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
 * @brief MPI Constants
 */
namespace Params {
    static constexpr int master_id = 0;
    static constexpr uint32_t domain_length = 1000;
    static constexpr uint32_t walker_max_steps = 100000;
    static constexpr uint32_t n_walkers_per_proc = 10000;
} /* namespace Params */

/** ---------------------------------------------------------------------------
 * @brief Plain structure holding the current position and the number of
 * remaining steps of a walker object.
 */
struct Walker {
    uint32_t position;
    uint32_t n_steps;

    static std::vector<Walker> Create(
        const uint32_t n_walkers_per_proc,
        const uint32_t walker_max_steps,
        const uint32_t start_position);
};

/**
 * @brief Walker factory function. Create a collection of walkers, each with
 * a random number of steps determining its lifetime.
 */
std::vector<Walker> Walker::Create(
    const uint32_t n_walkers_per_proc,
    const uint32_t walker_max_steps,
    const uint32_t start_position)
{
    /* Walker steps random distribution */
    std::random_device seed;    /* rng device */
    std::mt19937 rng(seed());   /* rng engine */
    std::uniform_int_distribution<uint32_t> walker_steps(0, walker_max_steps);

    /* Generate walkers */
    std::vector<Walker> walkers;
    for (uint32_t i = 0; i < n_walkers_per_proc; ++i) {
        walkers.push_back({start_position, walker_steps(rng)});
    }
    return walkers;
}

/** ---------------------------------------------------------------------------
 * @brief Domain structure represents a bounded region with lower and upper
 * limits and corresponding size.
 */
struct Domain {
    uint32_t begin;
    uint32_t end;
    uint32_t length;

    static Domain Create(
        const uint32_t domain_length,
        const int32_t n_procs,
        const int32_t proc_id);
};

/**
 * @brief Domain factory function.
 */
Domain Domain::Create(
    const uint32_t domain_length,
    const int32_t n_procs,
    const int32_t proc_id)
{
    /*
     * This shouldn't happen - the domain length needs to be larger than the
     * total number of process so it can be partitioned in a correct way.
     */
    if (n_procs > (int32_t) domain_length) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    /*
     * Compute the domain minimum and maximum ranges for this process with a
     * corresponding proc_id < n_procs. If this is the last process in
     * the set, add the remainder of the domain to it.
     */
    uint32_t length = domain_length / n_procs;

    uint32_t min = proc_id * length;
    uint32_t max = min + length - 1;
    if (proc_id == n_procs - 1) {
        max += domain_length % n_procs;
    }

    return Domain{min, max, max - min + 1};
}

/** ---------------------------------------------------------------------------
 * Step
 * @brief Each walker with available steps takes a step.
 */
void Step(
    const Domain &domain,
    std::vector<Walker> &in_walkers,
    std::vector<Walker> &out_walkers,
    std::vector<Walker> &send_walkers)
{
    /* Move any walker that still has steps to take */
    for (auto &w : in_walkers) {
        if (w.n_steps > 0) {
            w.position++;
            w.n_steps--;
        }
    }

    /* Send any walker whose position is outside the domain. */
    out_walkers.clear();
    send_walkers.clear();
    for (auto &w : in_walkers) {
        if (w.position > domain.end) {
            send_walkers.push_back(w);
        } else {
            out_walkers.push_back(w);
        }
    }
}

/** ---------------------------------------------------------------------------
 * Send
 * @brief Send walkers to the next domain.
 * Send the data as an array of MPI_BYTEs to the next process.
 * The last process sends to process zero.
 */
void Send(std::vector<Walker> &send_walkers, const int n_procs, const int proc_id)
{
    MPI_Send(
        (void *) &send_walkers[0],
        send_walkers.size() * sizeof(send_walkers[0]),
        MPI_BYTE,
        (proc_id + 1) % n_procs,
        0,
        MPI_COMM_WORLD);
}

/** ---------------------------------------------------------------------------
 * Receive
 * @brief Receive walkers from the previous domain.
 */
void Receive(std::vector<Walker> &out_walkers, const int n_procs, const int proc_id)
{
    /*
     * Receive from the preceding process. If you are process zero, receive
     * from the last process.
     */
    MPI_Status status;
    int source_id = (proc_id == 0) ? n_procs - 1 : proc_id - 1;
    MPI_Probe(source_id, 0, MPI_COMM_WORLD, &status);

    /* Resize the in_walkers buffer based on how much data is being received. */
    int in_walkers_size;
    MPI_Get_count(&status, MPI_BYTE, &in_walkers_size);
    std::vector<Walker> in_walkers(in_walkers_size / sizeof(Walker));
    MPI_Recv(
        (void *) &in_walkers[0],
        in_walkers_size,
        MPI_BYTE,
        source_id,
        0,
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);

    /* Append the received walkers into the output buffer */
    out_walkers.insert(out_walkers.end(), in_walkers.begin(), in_walkers.end());
}

/** ---------------------------------------------------------------------------
 * main
 */
int main (int argc, char *argv[])
{
    /* ------------------------------------------------------------------------
     * Initialize MPI context
     */
    MPI_Init(&argc, &argv);

    /* Get MPI world size and rank */
    int n_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    int proc_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    /* ------------------------------------------------------------------------
     * Create the subdomain corresponding to this process.
     */
    Domain domain = Domain::Create(Params::domain_length, n_procs, proc_id);

    /* Create a double buffer  */
    std::array<std::vector<Walker>, 2> walkers;
    size_t read_ix = 0;
    size_t write_ix = 1;
    walkers[write_ix] = Walker::Create(
        Params::n_walkers_per_proc, Params::walker_max_steps, domain.begin);
    std::vector<Walker> send_walkers;

    /* ------------------------------------------------------------------------
     * Run model
     */
    size_t step = 0;
    while (step++ < Params::walker_max_steps) {
        /* Swap input/output buffers */
        std::swap(read_ix, write_ix);

        /* Take a step and send any walkers that stepped outside the domain. */
        Step(domain, walkers[read_ix], walkers[write_ix], send_walkers);

        /*
         * Send outgoing walkers to the next domain and receive incomming
         * walkers from the previous domain. The order of send/receive calls
         * of even process ids is the reverse of the order of odd process ids
         * to avoid a deadlock scenario where every process is trying to send
         * simultaneously to its neighbour without ever returning.
         */
        if (proc_id % 2 == 0) {
            Send(send_walkers, n_procs, proc_id);
            Receive(walkers[write_ix], n_procs, proc_id);
        } else {
            Receive(walkers[write_ix], n_procs, proc_id);
            Send(send_walkers, n_procs, proc_id);
        }
    }



    /* ------------------------------------------------------------------------
     * Teardown model.
     */
    {
        size_t n_walkers = walkers[read_ix].size();
        if (proc_id == 0) {
            for (int source_id = 1; source_id < n_procs; ++source_id) {
                size_t count;
                MPI_Recv(
                    (void *) &count,
                    sizeof(size_t),
                    MPI_BYTE,
                    source_id,
                    0,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
                n_walkers += count;
                printf("%lu walkers from proc %d, total %lu\n", count, source_id, n_walkers);
            }
        } else {
            MPI_Send(
                (void *) &n_walkers,
                sizeof(size_t),
                MPI_BYTE,
                0,
                0,
                MPI_COMM_WORLD);
        }
    }
    // printf("Process %d of %d with %lu walkers\n", proc_id, n_procs, walkers[read_ix].size());



    /* ------------------------------------------------------------------------
     * Finalize MPI context
     */
    MPI_Finalize();

    /* Done */
    exit(EXIT_SUCCESS);
}
