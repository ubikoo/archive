#! /bin/bash

#
# make-test.sh
#
# Copyright (c) 2020 Carlos Braga
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the MIT License.
#
# See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
#

# -----------------------------------------------------------------------------
#
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
#
# run test
#
#TEST='./test.out -s -v normal'
TEST='./test.out'

#
# Test algorithms and data structures
#
runtest() {
    pushd "$1"
    run make clean > /dev/null
    run make -j16 all > /dev/null
    ./test.out
    run make clean > /dev/null
    popd
}

runtest ../algo/ds/array/test           # Test array data structures
runtest ../algo/ds/linked/test          # Test linked data structures

runtest ../algo/graphs/test             # Test graph algorithms
runtest ../algo/search/test             # Test search algorithms
runtest ../algo/sort/test               # Test sort algorithms
runtest ../algo/strings/test            # Test string algorithms

runtest ../algo/numeric/array/test      # Test numeric arrays
runtest ../algo/numeric/histogram/test  # Test histogram
runtest ../algo/numeric/linalg/test     # Test linear algebra
runtest ../algo/numeric/ode/test        # Test numeric ode solvers
runtest ../algo/numeric/sde/test        # Test stochastic solvers
runtest ../algo/numeric/transform/test  # Test Fourier and spherical harmonics

