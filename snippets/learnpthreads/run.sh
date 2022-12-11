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
    run ./run.out
    run make -f ../Makefile clean
    popd
}

execute 01-create-join
execute 02-mutex
execute 03-mutex
execute 04-mutex
execute 05-condition
execute 06-condition
execute 07-condition
execute 08-condition
execute 09-matrix-multiply-serial
execute 09-matrix-multiply-thread
execute 10-threadpool
execute 11-threadpool
execute 12-threadpool-pi
execute 13-threadpool-pi
