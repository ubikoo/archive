/*
 * file.h
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_FILE_H_
#define CORE_FILE_H_

#include "base.h"
#include "array.h"
#include "string.h"

/** ---- File class -----------------------------------------------------------
 *
 *                         --> File <--
 *                       /              \
 *                  FileStdIn        FileStdOut
 *                     ^                 ^
 *                    |                 |
 *                 FileIn            FileOut
 *
 * struct File
 *
 * @brief File is a struct representing a C-style FILE stream.
 *
 * @note Unlike the C++ implementation, the names FileStdIn and FileIn,
 * FileStdOut and FileOut, are not classes but represent an input/output
 * abstraction of the C I/O interface using the File structure.
 *
 * @note
 * C-streams are objects of type FILE that can only be accessed and
 * manipulated through pointers of type FILE*.
 * Important: while it may be possible to create a local object of type
 * FILE by dereferencing and copying a valid FILE*, using the
 * address of such copy in the I/O functions is undefined behavior.
 *
 * @see
 * https://en.cppreference.com/w/cpp/io/c
 */
struct File {
    enum MODE {
        StdIn  = (1 << 0),
        In     = (1 << 1),
        StdOut = (1 << 2),
        Out    = (1 << 3),
        Binary = (1 << 4),
    } mode;            /* file access mode */
    FILE *fp;          /* pointer to file object */
};

/*
 * C linkage declarations
 */
#undef  __BEGIN_DECLS
#undef  __END_DECLS
#ifdef  __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS   /* empty */
#define __END_DECLS     /* empty */
#endif
__BEGIN_DECLS
/**
 * File query functions.
 */
bool is_file_open (const struct File *file);
bool is_file_stdin (const struct File *file);
bool is_file_in (const struct File *file);
bool is_file_stdout (const struct File *file);
bool is_file_out (const struct File *file);
bool is_file_binary (const struct File *file);
bool is_file_good (const struct File *file);
bool is_file_error (const struct File *file);
bool is_file_eof (const struct File *file);
void file_clearerr (const struct File *file);

/**
 * File input interface.
 */
struct File *create_file_stdin (void);
struct File *create_file_in (const char *filename, const bool binary);
void file_in_open (struct File *file, const char *filename, const bool binary);
void file_in_close (struct File *file);
bool file_in_readline (
    struct File *file,
    struct String *line,
    size_t limit,
    const char delim,
    const char ignore);
bool file_in_readlines (
    struct File *file,
    struct ArrayVoid *lines,
    size_t hint,
    const char delim,
    const char ignore);
int64_t file_in_length (struct File *file);
bool file_in_read (struct File *file, void *ptr, size_t size);

/**
 * File output interface.
 */
struct File *create_file_stdout (void);
struct File *create_file_out (const char *filename, const bool binary);
void file_out_open (struct File *file, const char *filename, const bool binary);
void file_out_close (struct File *file);
bool file_out_writeline (
    struct File *file,
    const struct String *line,
    const struct String *sep);
bool file_out_writelines (
    struct File *file,
    struct ArrayVoid *lines,
    const struct String *sep);
int64_t file_out_length (struct File *file);
bool file_out_write (struct File *file, void *ptr, size_t size);

__END_DECLS

#endif /* CORE_FILE_H_ */
