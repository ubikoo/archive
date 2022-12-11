#! /bin/bash

#
# runtest.sh
#
# Copyright (c) 2020 Carlos Braga
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the MIT License.
#
# See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
#

# -----------------------------------------------------------------------------
# die with message
#
die() {
    echo >&2 "$@"
    exit 1
}

#
# run command and check exit code
#
run() {
    echo "$@" && "$@"
    code=$?
    [[ $code -ne 0 ]] && die "[$@] failed with error code $code"
    return 0
}

#
# ask for input query
#
ask() {
    echo -n "$@ (y/n [n]): "
    local ans
    read ans
    [[ "$ans" != "y" ]] && return 1
    return 0
}

# -----------------------------------------------------------------------------
# Execute
#
execute() {
    pushd "${1}"
    run make -f ../Makefile clean
    run make -f ../Makefile all
    run mpirun --mca shmem posix --use-hwthread-cpus -np 16 run.out
    run make -f ../Makefile clean
    popd
}


execute 01-init-finalize
execute 02-send-recv
execute 03-send-recv-ring
execute 04-send-recv-pingpong
execute 05-send-recv-status
execute 06-send-recv-probe
execute 07-send-recv-randomwalk
execute 08-scatter-gather
execute 09-scatter-allgather
execute 10-reduce
execute 11-allreduce
