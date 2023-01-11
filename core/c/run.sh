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
# Test core
pushd tests
run make clean && make -j16 all && ./test.out
make clean

run diff data/lorem_ipsum.txt data/lorem_ipsum_new.txt
rm -v data/lorem_ipsum_new.txt
popd
