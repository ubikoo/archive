/*
 * main.c
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 *
 *  See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "test_array.h"
#include "test_string.h"
#include "test_file.h"

/* ---- Main test client -----------------------------------------------------
 * main
 */
int main (int argc, char const *argv[])
{
    /* Run tests */
    test_array();
    test_string();
    test_file();

   /* Done */
    exit(EXIT_SUCCESS);
}
