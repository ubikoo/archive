/*
 * test_file.c
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 *
 *  See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "test_file.h"

/** ---------------------------------------------------------------------------
 * File test client
 */
void test_file (void)
{
    /*
     * Read file
     */
    struct File *fin = create_file_in("data/lorem_ipsum.txt", false);
    struct ArrayVoid *lines = create_array_void();
    file_in_readlines(fin, lines, -1, '\n', '\0');
    file_in_close(fin);

    /*
     * Write file
     */
    struct String *sep = create_string("\n");
    struct File *fout = create_file_out("data/lorem_ipsum_new.txt", false);
    file_out_writelines(fout, lines, sep);
    destroy_string(sep);
    file_out_close(fout);

    /*
     * Print out file
     */
    while (!array_void_is_empty(lines)) {
        struct String *it = (struct String *) array_void_pop(lines);
        printf("%s\n", it->data);
        destroy_string(it);
    }
    destroy_array_void(lines);
}
