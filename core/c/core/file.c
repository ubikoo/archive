/*
 * file.c
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "file.h"

/** ---- File query functions -------------------------------------------------
 *
 * is_file_open
 * @brief is the file open?
 */
bool is_file_open (const struct File *file)
{
    core_assert_(file != NULL, "null file object");
    return (file->fp != NULL);
}

/**
 * is_file_stdin
 * @brief Is the file stream stdin?
 */
bool is_file_stdin (const struct File *file)
{
    core_assert_(file != NULL, "null file object");
    return !!(file->mode & StdIn);
}


/**
 * is_file_in
 * @brief Is the file stream input?
 */
bool is_file_in (const struct File *file)
{
    core_assert_(file != NULL, "null file object");
    return !!(file->mode & In);
}

/**
 * is_file_stdout
 * @brief Is the file stream stdout?
 */
bool is_file_stdout (const struct File *file)
{
    core_assert_(file != NULL, "null file object");
    return !!(file->mode & StdOut);
}

/**
 * is_file_out
 * @brief Is the file access mode read?
 */
bool is_file_out (const struct File *file)
{
    core_assert_(file != NULL, "null file object");
    return !!(file->mode & Out);
}

/**
 * is_file_binary
 * @brief Is the file access mode binary?
 */
bool is_file_binary (const struct File *file)
{
    core_assert_(file != NULL, "null file object");
    return !!(file->mode & Binary);
}

/**
 * is_file_good
 * @brief Return true if most recent I/O stream operation was successful.
 * Specifically, check whether the error and the eof indicators associated
 * with an open file stream are set.
 */
bool is_file_good (const struct File *file)
{
    core_assert_(file != NULL, "null file object");
    return !(is_file_error(file) || is_file_eof(file));
}

/**
 * is_file_error
 * @brief Check whether the error indicator associated with the stream
 * is set, given by error return value - non zero if it is.
 */
bool is_file_error (const struct File *file)
{
    core_assert_(file != NULL, "null file object");
    if (is_file_open(file)) {
        return (ferror(file->fp) != 0);
    }
    return false;
}

/**
 * is_file_eof
 * @brief Check whether the eof indicator associated with the stream
 * is set, given by eof return value - non zero if it is.
 */
bool is_file_eof (const struct File *file)
{
    core_assert_(file != NULL, "null file object");
    if (is_file_open(file)) {
        return (feof(file->fp) != 0);
    }
    return false;
}

/**
 * file_clearerr
 * @brief Reset both error and eof indicators associated with the stream.
 */
void file_clearerr (const struct File *file)
{
    core_assert_(file != NULL, "null file object");
    if (is_file_open(file)) {
        clearerr(file->fp);
    }
}


/** ---- File input interface -------------------------------------------------
 *
 * create_file_stdin
 *
 * @brief Open a standard input file stream.
 */
struct File *create_file_stdin (void)
{
    struct File *file = (struct File *) calloc(1, sizeof(struct File));
    core_assert_(file != NULL, "failed to alloc file\n");
    file->mode = StdIn;
    file->fp = stdin;
    return file;
}

/**
 * create_file_in
 *
 * @brief Open an input file stream for reading.
 */
struct File *create_file_in (const char *filename, const bool binary)
{
    core_assert_(filename != NULL, "null filename");

    struct File *file = (struct File *) calloc(1, sizeof(struct File));
    core_assert_(file != NULL, "failed to alloc file\n");
    file->mode = In;
    file_in_open(file, filename, binary);
    return file;
}

/**
 * file_in_open
 *
 * @brief Open an input stream for reading in a given binary mode.
 * The filename is a non null string with the name of the file.
 * The flag binary is a boolean flag specifying the file binary mode.
 */
void file_in_open (struct File *file, const char *filename, const bool binary)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(is_file_in(file), "invalid file I/O type");
    core_assert_(!is_file_open(file), "file stream is already open");
    core_assert_(filename != NULL, "null filename");

    /* Set file mode and open file stream. */
    if (binary) {
        file->mode |= Binary;
    } else {
        file->mode &= ~Binary;
    }

    const char *mode = (is_file_binary(file) ? "rb" : "r");
    file->fp = fopen(filename, mode);
    core_assert_(
        is_file_open(file),
        "failed to open %s, mode %s\n", filename, mode);
}

/**
 * file_in_close
 *
 * @brief Close an input stream. Any unread buffered data are discarded.
 *
 * If stream is successfully closed, a zero value is returned, otherwise
 * EOF is returned. Even if the call fails, the stream will no longer be
 * associated with the file.
 */
void file_in_close (struct File *file)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(is_file_in(file), "invalid file I/O type");

    /* File pointer is not attached, nothing to do. */
    if (!is_file_open(file)) {
        return;
    }

    /* Close the file stream and reset the file pointer. */
    fclose(file->fp);
    file->fp = NULL;
}

/**
 * file_in_readline
 *
 * @brief Read a line from an input stream and strip newline characters.
 *
 * Stop when either:
 *  limit characters are read (default max(size_t)), or
 *  delim character is read (default newline '\n'), or
 *  end of file EOF is reached, whichever comes first.
 *
 * Any characters read after an ignore marker (default '\0') are ignored.
 * Return the stream state after reading.
 *
 * @note (C version) ----------------------------------------------------------
 *
 *  int c;      // note: int, not char, required to handle EOF
 *  while ((c = fgetc(fp)) != EOF) {
 *      putchar(c);
 *  }
 *
 *  if (ferror(fp))
 *      puts("I/O error when reading");
 *  else if (feof(fp))
 *      puts("End of file reached successfully");
 *
 * The function signature,
 *
 *  int fgetc(FILE* stream);
 *
 * returns the obtained character on success or EOF on failure.
 *
 * If the failure has been caused by end of file condition,
 * additionally sets the eof indicator (see feof()) on stream.
 *
 * If the failure has been caused by some other error, sets the error
 * indicator (see ferror()) on stream.
 *
 * @note (C++ version) --------------------------------------------------------
 *
 *  char c;
 *  while (stream.get(c)) {
 *      cout << c;
 *  }
 *
 * The function signature,
 *
 *  int_type get();
 *
 * reads and returns one character. Otherwise, returns Traits::eof()
 * and sets failbit and eofbit.
 *
 * The function signature
 *
 *  basic_istream& get( char_type& ch );
 *
 * reads one character and stores it to ch if available. Otherwise,
 * leaves c unmodified and sets failbit and eofbit.
 *
 * @note (Character handling) -------------------------------------------------
 *
 * Handle null character '\0':
 * string automatically appends a null character '\0' to
 * the written string, even if an empty string is extracted.
 *
 * Handle newline character '\n':
 * Instead of strlen which has undefined behaviour for an
 * empty buffer beginning with '\0',
 *
 * if (*(str + strlen(str) - 1) == '\n')
 *     *(str + strlen(str) - 1) = '\0';
 *
 * strcspn counts the number of characters until it finds
 * a '\n' character or a '\r' character.
 *
 * If none found, it stops at the null terminator '\0' and returns 0.
 * In the later case, the operation is redudant but safe by simply
 * replacing character '\0' with the same character '\0'.
 *
 * str[strcspn(str, "\n")] = '\0';
 *
 * Alternatively, the function strchr returns a pointer to the
 * matched character '\n' or null if none is found.
 *
 * @see
 * https://isocpp.org/wiki/faq/input-output
 * https://en.cppreference.com/w/cpp/io/c/fgetc
 * https://en.cppreference.com/w/cpp/io/basic_istream/get
 * http://www.cplusplus.com/reference/istream/istream/get/
 * https://stackoverflow.com/questions/8558907
 */
bool file_in_readline (
    struct File *file,
    struct String *line,
    size_t limit,
    const char delim,
    const char ignore)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(
        is_file_stdin(file) || is_file_in(file), "invalid file I/O type");
    core_assert_(is_file_open(file), "file stream is not open");
    core_assert_(is_file_good(file), "file stream is not good");
    core_assert_(!is_file_binary(file), "file stream mode is binary");
    core_assert_(line != NULL, "null line buffer");

    /*
     * Read a line from the input stream one character at a time.
     */
    bool isignore = false;          /* set true if ignore character was read */
    string_clear(line);             /* clear line buffer before reading */
    int c;                          /* note: use int, not char, to handle EOF */
    while ((c = fgetc(file->fp)) != EOF) {
        if (c == delim) {
            break;                  /* read newline or end character */
        }

        if (c == ignore) {
            isignore = true;        /* ignore any further characters */
        }

        if (!isignore) {
            string_push(line, c);   /* store character in the buffer */
        }

        if (line->size == limit) {
            break;                  /* read limit number of characters */
        }
    }

    /* Return the stream state. */
    return is_file_good(file);
}

/**
 * file_in_readlines
 *
 * @brief Read at most 'hint' lines from a stream using readline() and
 * populate the 'lines' array with the strings thus read.
 *
 * Stop when either:
 *  buffer number of lines reaches hint value, or
 *  end-of-file is reached, whichever comes first.
 *
 * Each line reads characters from the input stream until a delim
 * character is read (default newline '\n').
 * Any characters read after ignore marker (default '\0') are ignored.
 */
bool file_in_readlines (
    struct File *file,
    struct ArrayVoid *lines,
    size_t hint,
    const char delim,
    const char ignore)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(
        is_file_stdin(file) || is_file_in(file), "invalid file I/O type");
    core_assert_(is_file_open(file), "file stream is not open");
    core_assert_(is_file_good(file), "file stream is not good");
    core_assert_(!is_file_binary(file), "file stream mode is binary");
    core_assert_(lines != NULL, "null line buffer");

    /*
     * Read lines from stream and store them in the buffer.
     */
    const size_t limit = SIZE_MAX;              /* maximum chars to be read */

    array_void_clear(lines);                    /* clear the buffer contents */
    struct String *line = create_string(NULL);  /* create a line buffer */
    while (
        lines->size < hint &&
        file_in_readline(file, line, limit, delim, ignore)) {
        array_void_push(lines, string_clone(line));
    }
    destroy_string(line);

    /* Return the stream state. */
    return is_file_good(file);
}

/**
 * file_in_length
 *
 * @brief Return the length of a given stream in bytes. On failure, return -1.
 *
 * For binary streams, this is the number of bytes from the beginning of
 * the file. For text streams, the numerical value may not be meaningful.
 *
 * @note (C++ version)
 * fstream has a joint file position maintained for both the input
 * sequence and the output sequence.
 * filebuf, the buffer used by fstream, has only one pointer.
 *
 * The functions seekg() and seekp() move the same pointer, and tellg()
 * and tellp() return its position.
 *
 * @see
 * https://stackoverflow.com/questions/19958679
 */
int64_t file_in_length (struct File *file)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(is_file_in(file), "invalid file I/O type");
    core_assert_(is_file_open(file), "file stream is not open");
    core_assert_(is_file_good(file), "file stream is not good");
    core_assert_(is_file_binary(file), "file stream is not binary");

    /* Compute the file length in bytes.  */
    fseek(file->fp, 0, SEEK_END);       /* go to end of file */
    int64_t len = ftell(file->fp);      /* get length in bytes */
    fseek(file->fp, 0, SEEK_SET);       /* go back to begining of file */
    return len;
}

/**
 * file_in_read
 *
 * @brief Read one block of size bytes from the input stream as if by
 * calling fgetc size times, and storing the results, in the order
 * obtained, into the successive positions of buffer.
 *
 * The number of objects successfully read may be less than count 1 if
 * an error or end-of-file condition occurs. This is asserted by the
 * stream state return value.
 *
 * @note
 * Read one block with size bytes from the buffer stream if
 *
 * file pointer is attached, and
 * file pointer state is good, and
 * ptr is not NULL (undefined behaviour)
 *
 * (C++ version)
 *  if (!fp.read(static_cast<char *>(ptr), size)) {
 *      throw runtime_error("I/O read error");
 *  }
 */
bool file_in_read (struct File *file, void *ptr, size_t size)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(is_file_in(file), "invalid file I/O type");
    core_assert_(is_file_open(file), "file stream is not open");
    core_assert_(is_file_good(file), "file stream is not good");
    core_assert_(is_file_binary(file), "file stream mode is not binary");
    core_assert_(ptr != NULL, "invalid pointer");
    core_assert_(size > 0, "invalid size");

    /* Read one block of size bytes from the file stream. */
    fread(ptr, size, 1, file->fp);

    /* Return the stream state. */
    return is_file_good(file);
}


/** ---- File output interface ------------------------------------------------
 *
 * create_file_stdout
 *
 * @brief Open a standard output file stream.
 */
struct File *create_file_stdout (void)
{
    struct File *file = (struct File *) calloc(1, sizeof(struct File));
    core_assert_(file != NULL, "failed to alloc file\n");
    file->mode = StdOut;
    file->fp = stdout;
    return file;
}

/**
 * create_file_out
 *
 * @brief Open an output file stream for writing.
 */
struct File *create_file_out (const char *filename, const bool binary)
{
    core_assert_(filename != NULL, "null filename");

    struct File *file = (struct File *) calloc(1, sizeof(struct File));
    core_assert_(file != NULL, "failed to alloc file\n");
    file->mode = Out;
    file_out_open(file, filename, binary);
    return file;
}

/**
 * file_out_open
 *
 * @brief Open an input stream for writing in a given binary mode.
 *
 * filename is a non null string with the name of the file.
 * binary is a boolean flag asserting the file binary mode.
 */
void file_out_open (struct File *file, const char *filename, const bool binary)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(is_file_out(file), "invalid file I/O type");
    core_assert_(!is_file_open(file), "file stream is already open");
    core_assert_(filename != NULL, "null filename");

    /* Set file mode and open file stream. */
    if (binary) {
        file->mode |= Binary;
    } else {
        file->mode &= ~Binary;
    }

    const char *mode = (is_file_binary(file) ? "wb" : "w");
    file->fp = fopen(filename, mode);
    core_assert_(
        is_file_open(file),
        "failed to open %s, mode %s\n", filename, mode);
}

/**
 * file_out_close
 *
 * @brief Close an input stream.
 *
 * If the stream is successfully closed, a zero value is returned.
 * Otherwise end-of-file is returned. Even if the call fails, the
 * stream will no longer be associated with the file.
 *
 * Any unwritten buffered data are flushed with fflush.
 * Flushing an input stream is undefined behaviour.
 *
 * @see
 * https://en.cppreference.com/w/cpp/io/c/fflush
 */
void file_out_close (struct File *file)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(is_file_out(file), "invalid file I/O type");

    /* File pointer is not attached, nothing to do. */
    if (!is_file_open(file)) {
        return;
    }

    /* Close the file stream and reset the file pointer. */
    fflush(file->fp);
    fclose(file->fp);
    file->fp = NULL;
}

/**
 * file_out_writeline
 *
 * Write every character from the null terminated string line
 * to the output stream, as if by repeatedly executing fputc.
 * The terminating null character from is not written.
 *
 * The argument sep provides a line separator.
 */
bool file_out_writeline (
    struct File *file,
    const struct String *line,
    const struct String *sep)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(
        is_file_stdout(file) || is_file_out(file), "invalid file I/O type");
    core_assert_(is_file_open(file), "file stream is not open");
    core_assert_(is_file_good(file), "file stream is not good");
    core_assert_(!is_file_binary(file), "file stream mode is binary");
    core_assert_(line != NULL, "null line buffer");
    core_assert_(sep != NULL, "null line separator");

    /*
     * Write the line if it contains data. If stream state
     * is valid, append the separator if it is not empty.
     */
    if (!string_is_empty(line)) {
        fputs(line->data, file->fp);
    }

    if (is_file_good(file) && !string_is_empty(sep)) {
        fputs(sep->data, file->fp);
    }

    /* Return the stream state. */
    return is_file_good(file);
}

/**
 * File::writelines
 *
 * Write lines to the stream using writeline().
 */
bool file_out_writelines (
    struct File *file,
    struct ArrayVoid *lines,
    const struct String *sep)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(
        is_file_stdout(file) || is_file_out(file), "invalid file I/O type");
    core_assert_(is_file_open(file), "file stream is not open");
    core_assert_(is_file_good(file), "file stream is not good");
    core_assert_(!is_file_binary(file), "file stream mode is binary");
    core_assert_(lines != NULL, "null line buffer");
    core_assert_(sep != NULL, "null line separator");

    /*
     * Try writing lines to the stream.
     */
    struct String **it = (struct String **) lines->data;
    struct String **end = it + lines->size;
    while (it < end && file_out_writeline(file, *it, sep)) {
        it++;
    }

    /* Return the stream state. */
    return is_file_good(file);
}

/**
 * file_out_length
 *
 * @brief Return the length of a given stream in bytes. On failure, return -1.
 *
 * For binary streams, this is the number of bytes from the beginning of
 * the file. For text streams, the numerical value may not be meaningful.
 *
 * @note (C++ version)
 * fstream has a joint file position maintained for both the input
 * sequence and the output sequence.
 * filebuf, the buffer used by fstream, has only one pointer.
 *
 * The functions seekg() and seekp() move the same pointer, and tellg()
 * and tellp() return its position.
 *
 * @see
 * https://stackoverflow.com/questions/19958679
 */
int64_t file_out_length (struct File *file)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(is_file_out(file), "invalid file I/O type");
    core_assert_(is_file_open(file), "file stream is not open");
    core_assert_(is_file_good(file), "file stream is not good");
    core_assert_(is_file_binary(file), "file stream is not binary");

    /* Compute the file length in bytes. */
    fseek(file->fp, 0, SEEK_END);       /* go to end of file */
    int64_t len = ftell(file->fp);      /* get length in bytes */
    fseek(file->fp, 0, SEEK_CUR);       /* go to current file position */
    return len;
}

/**
 * file_out_write
 *
 * @brief Write one block of size bytes from the input stream as if
 * by interpreting the block as an array of unsigned char and calling
 * fputc size times to write those unsigned chars into stream.
 *
 * The file position indicator for the stream is advanced by the
 * number of characters.
 *
 * @note Write one block with size bytes into the buffer stream if:
 *
 * file pointer is attached, and
 * file pointer state is good, and
 * ptr is not NULL (undefined behaviour)
 *
 * (C++ version)
 *  if (!fp.write(static_cast<char *>(ptr), size)) {
 *      throw runtime_error("I/O write error");
 *  }
 */
bool file_out_write (struct File *file, void *ptr, size_t size)
{
    /* Assert I/O preconditions. */
    core_assert_(file != NULL, "null file object");
    core_assert_(is_file_out(file), "invalid file I/O type");
    core_assert_(is_file_open(file), "file stream is not open");
    core_assert_(is_file_good(file), "file stream is not good");
    core_assert_(is_file_binary(file), "file stream mode is not binary");
    core_assert_(ptr != NULL, "invalid pointer");
    core_assert_(size > 0, "invalid size");

    /* Write one block of size bytes to the file stream. */
    fwrite(ptr, size, 1, file->fp);

    /* Return the stream state. */
    return is_file_good(file);
}
