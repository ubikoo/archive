/*
 * string.c
 *
  * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "string.h"

/** ---- String array implementation ------------------------------------------
 *
 * create_string
 * @brief Create an string with n>=0 items.
 */
struct String *create_string (const char *source)
{
    struct String *string = (struct String *) calloc(1, sizeof(struct String));
    core_assert_(string != NULL, "failed to alloc string\n");
    string_clear(string);
    if (source != NULL) {
        string_c_copy(string, source);
    }
    return string;
}

/**
 * destroy_string
 * @brief Destroy the string and all items.
 */
void destroy_string (struct String *string)
{
    core_assert_(string != NULL, "invalid string");
    free(string->data);
    free(string);
}

/**
 * string_is_empty
 * @brief Is the string empty?
 */
bool string_is_empty (const struct String *string)
{
    core_assert_(string != NULL, "invalid string");
    return (string->size == 0);
}

/**
 * string_clear
 * @brief Remove all items from the string.
 */
void string_clear (struct String *string)
{
    core_assert_(string != NULL, "invalid string");

    free(string->data);
    string->data = (char *) calloc(1, sizeof(char));
    core_assert_(string->data != NULL, "failed to alloc string->data\n");

    *string->data = '\0';
    string->capacity = 1;
    string->size = 0;
}

/**
 * string_resize
 * @brief Resize the string.
 */
void string_resize (struct String *string, size_t capacity)
{
    core_assert_(string != NULL, "invalid string");
    core_assert_(capacity > 0, "invalid size");

    /*
     * Create a null terminated copy string with the new capacity.
     */
    char *copy = (char *) calloc(capacity, sizeof(char));
    core_assert_(copy != NULL, "failed to alloc string copy\n");
    memset(copy, '\0', capacity*sizeof(char));

    /*
     * Copy the items of the current string and swap the data.
     */
    string->capacity = capacity;
    string->size = string->size < capacity ? string->size : capacity;
    memcpy(copy, string->data, string->size*sizeof(char));
    free(string->data);
    string->data = copy;
}

/**
 * string_push
 * @brief Insert an item at the back.
 */
void string_push (struct String *string, char item)
{
    core_assert_(string != NULL, "invalid string");

    /*
     * The last element in the string is reserved for the null
     * termination character.
     * Resize the underlying char array when the string size plus
     * one for the null termination character equals its capacity.
     */
    if (string->size + 1 == string->capacity) {
        string_resize(string, 2 * string->capacity);
    }
    string->data[string->size] = item;
    ++string->size;
}

/**
 * string_pop
 * @brief Remove an item from the back.
 */
char string_pop (struct String *string)
{
    core_assert_(string != NULL, "invalid string");
    core_assert_(!string_is_empty(string), "empty string, out of range error");

    /*
     * Remove the character item from the back of the string.
     * If the string size plus 1 for the null termination char
     * is 1/4 of its current capacity, then we can halve the
     * string capacity and maintain a 2:1 ratio.
     */
    --string->size;
    char item = string->data[string->size];
    string->data[string->size] = '\0';
    if (string->size + 1 == (string->capacity / 4)) {
        string_resize(string, string->capacity / 2);
    }
    return item;
}

/**
 * string_c_copy
 * @brief Copies the c-string pointed to by source to the string.
 */
void string_c_copy (struct String *string, const char *source)
{
    core_assert_(string != NULL, "invalid string");
    core_assert_(source != NULL, "invalid source");

    string_clear(string);
    const char *p = source;
    while (*p != '\0') {
        string_push(string, *p++);
    }
}

/**
 * string_copy
 * @brief Copies the string pointed to by source to the string.
 */
void string_copy (struct String *string, const struct String *source)
{
    core_assert_(string != NULL, "invalid string");
    core_assert_(source != NULL, "invalid source");
    string_c_copy(string, source->data);
}

/**
 * string_c_append
 * @brief Appends a c-string pointed to by source to the string.
 */
void string_c_append (struct String *string, const char *source)
{
    core_assert_(string != NULL, "invalid string");
    core_assert_(source != NULL, "invalid source");

    const char *p = source;
    while (*p != '\0') {
        string_push(string, *p++);
    }
}

/**
 * string_append
 * @brief Appends a string pointed to by source to the string.
 */
void string_append (struct String *string, const struct String *source)
{
    core_assert_(string != NULL, "invalid string");
    core_assert_(source != NULL, "invalid source");
    string_c_append(string, source->data);
}

/**
 * string_clone
 * @brief Return a clone of the string.
 */
struct String *string_clone (const struct String *string)
{
    core_assert_(string != NULL, "invalid string");

    struct String *clone = create_string(string->data);
    return clone;
}


/** ---- String manipulation functions ----------------------------------------
 *
 * string_ltrim
 * @brief Trim off left spaces from the string.
 *
 * Find the first non space character or stop at the end.
 * Ensure the value isspace takes is of type unsigned char.
 */
char *string_ltrim (char * str)
{
    /* Scan from beginning and find the first non space character */
    char *beg = str;
    while ((*beg != '\0') && isspace((unsigned char) *beg)) {
        beg++;
    }

    /* Shift chars in place if needed */
    char *dst = str;
    if (beg != dst) {
        while ((*dst++ = *beg++));    /* stop at first null terminator */
    }

    return str;
}

/**
 * string_rtrim
 * @brief Trim off right spaces from the string.
 *
 * Start from the last character in the string and replace
 * any space character by a null terminator.
 * Stop at beginning or at the first non space character.
 * Ensure the value isspace takes is of type unsigned char.
 */
char *string_rtrim (char * str)
{
    /* Scan from end to beg and replace space by null terminator */
    char *beg = str;
    char *end = str + strlen(str) - 1;
    while ((end >= beg) && isspace((unsigned char) *end)) {
        *end-- = '\0';
    }

    return str;
}

/**
 * string_trim
 * @brief Trim off left and right spaces from the string.
 */
char *string_trim (char *str)
{
    return string_ltrim(string_rtrim(str));
}

/**
 * string_replace
 * @brief Scan the string and replace any characters given by search by
 * the characters given by replace.
 */
char *string_replace (char *str, const char search, const char replace)
{
    /* Check the trivial case */
    if (search == replace) {
        return str;
    }

    /* Scan the string for search and replace */
    char *tok = str;
    while ((tok = strchr(tok, search)) != NULL) {
        *tok++ = replace;
    }

    return str;
}

/**
 * string_uncomment
 * @brief Trim off string comments.
 *
 * Scan the string and remove all characters between the comment symbol '#'
 * and the end character.
 *
 * For a single line where the newline character has been replaced by the
 * null terminator, end='\0'.
 *
 * For a full stream buffer with newline characters, end='\n' should be
 * used instead.
 */
char *string_uncomment (char *str, const char end)
{
    /*
     * Find the first character that matches comment '#'
     * Replace all characters by space until the end
     */
    char *beg = str;
    while ((beg = strchr(beg, '#')) != NULL) {
        char *tok = beg++;
        while ((*tok != '\0') && (*tok != end)) {
            *tok++ = '\0';
        }
    }

    return str;
}

/**
 * string_numcpy
 * @brief Copy the first num-1 characters of source word to destination str.
 *
 * If the source is shorter than num-1 characters, the destination is padded
 * with zeros until a total num characters have been written to it.
 *
 * Unlike strncpy, a null character is implicitly appended at the end of
 * destination if source is shorter than num.
 */
char *string_numcpy (char *str, const char *word, long num)
{
    /*
     * Copy the first num-1 characters up to the
     * source string length to the destination...
     */
    long len = strlen(word);
    char *dst = str;
    while ((dst-str < len) && (dst-str < num-1)) {
        *dst++ = *word++;
    }

    /*
     * ...and pad the remaining destination characters
     * with zeros until num characters have been written
     */
    while (dst-str < num) {
        *dst++ = '\0';
    }

    /* Return destination string */
    return str;
}


/** ---- String token functions -----------------------------------------------
 *
 * string_next_token
 * @brief Retrive the next token found in a string.
 *
 * Unlike strtok, this function does not modify the input string
 * and copies instead the next token found into the word argument.
 *
 * If word = NULL, the  copy is not performed.
 *
 * If the word length is shorter than the token length, only the
 * first num-1 characters are copied.
 *
 * A null terminator is implictly added at the end.
 *
 * The implicit delimiter separating the string tokens is white-space
 * character as given by the "C" locale:
 *
 * ' '  (0x20)    space (SPC)
 * '\t' (0x09)    horizontal tab (TAB)
 * '\n' (0x0a)    newline (LF)
 * '\v' (0x0b)    vertical tab (VT)
 * '\f' (0x0c)    feed (FF)
 * '\r' (0x0d)    carriage return (CR)
 *
 * The function returns a pointer to the next token. A null pointer
 * is returned if there are no tokens left in the substring or an
 * EOF is reached.
 */
char *string_next_token (const char *str)
{
    /* Token of the first non space character */
    char *beg = (char *) str;
    while ((*beg != '\0') && isspace((unsigned char) *beg)) {
        beg++;
    }
    if (*beg == '\0') {
        return NULL;        /* reached end-of-string */
    }

    /* Token of the next space character */
    char *end = beg;
    while ((*end != '\0') && !isspace((unsigned char) *end)) {
        end++;
    }

    return end;
}

/**
 * string_count_tokens
 * @brief Count the number of tokens delimited by white space in a string.
 */
size_t string_count_tokens (char *str)
{
    size_t count = 0;
    char *cur = str;
    while ((cur = string_next_token(cur)) != NULL) {
          ++count;
    }
    return count;
}

/**
 * string_split_tokens
 * @brief Split the string into tokens delimited by white space.
 *
 * Scan first the string to count the total number of tokens and create
 * a new token structure with a maximum word length given by the argument
 * length.
 *
 * Scan the string again and store all the tokens found into the newly
 * created object.
 */
char **string_split_tokens (char *str, size_t *n_tokens)
{
    /* Do not split a null string */
    if (str == NULL) {
        return NULL;
    }

    /* Scan the string and count number of token words */
    *n_tokens = string_count_tokens(str);
    if (*n_tokens == 0) {
        return NULL;
    }

    /* Create an array of strings to store all the tokens */
    char **tokens = (char **) calloc(*n_tokens, sizeof(char *));
    core_assert_(tokens != NULL, "fai   led to alloc tokens");

    /* Rescan the string and store the token words */
    size_t ix = 0;
    char *end = str;
    char *beg = end;
    while ((end = string_next_token(beg)) != NULL) {
        /* Create a new token word with num+1 chars including null terminator */
        int num = (end - beg) / sizeof(char);
        char *word = (char *) calloc(num+1, sizeof(char));
        core_assert_(word != NULL, "fai led to alloc word");

        /* Copy the token characters */
        char *src = beg;
        char *dst = word;
        while ((dst-word < num) && (src < end)) {
            *dst++ = *src++;
        }
        *dst = '\0';

        /* Store the token word */
        tokens[ix++] = word;

        /* Next token */
        beg = end;
    }

    return tokens;
}

/**
 * split_tokens
 *
 * @brief Split the string into tokens delimited by white space.
 *
 * Scan the string first and count the total number of tokens.
 * If the string has no tokens, do not clear the current token set.
 * Otherwise, rescan the string and store each token sequentially.
 */
struct String **split_tokens (char *str, size_t *n_tokens)
{
    /* Do not split a null string */
    if (str == NULL) {
          return NULL;
    }

    /* Scan the string and count number of token words */
    *n_tokens = string_count_tokens(str);
    if (*n_tokens == 0) {
        return NULL;
    }

    /* Create an array of strings to store all the tokens */
    struct String **tokens = (struct String **) calloc(
        *n_tokens,
        sizeof(struct String *));
    core_assert_(tokens != NULL, "fai   led to alloc tokens");

    /* Scan the string and store the token words */
    size_t ix = 0;
    char *end = str;
    char *beg = end;
    while ((end = string_next_token(beg)) != NULL) {
          /* Create a new token word and copy the token characters */
        tokens[ix] = create_string(NULL);
        char *cur = beg;
        while (cur < end) {
            string_push(tokens[ix], *cur++);
        }

        /* Next token */
        beg = end;
    }
    return tokens;
}


/** ---- String cast conversion functions -------------------------------------
 *
 * @brief Parse the string interpreting its content as a number.
 * The cast helper functions handle the string conversion to the
 * fundamental data types:
 *
 * (floating point types)
 * lond double, double, float
 *
 * (integer types, signed and unsigned)
 * long long, long, int, short
 *
 * The cast function names are defined using the OpenGL suffix
 * convention {ld, d, f, s,, i, l, ll, us, ui, ul, ull}.
 *
 * These functions handle success or failure reported by the
 * corresponding strto functions - endptr and errno.
 *
 * If the endptr is not NULL, a pointer to the character after
 * the last used in the conversion is stored in the location
 * referenced by endptr.
 *
 * If the correct value would cause overflow, plus or minus HUGE_VAL,
 * HUGE_VALF, or HUGE_VALL is returned - according to the sign and type
 * of the return value; and ERANGE is stored in errno.
 *
 * If the correct value would cause underflow, zero is returned and
 * ERANGE is stored in errno.
 */

/**
 * string_castld
 * @brief Parse the string as a long double.
 */
long double string_castld (const char *str)
{
    errno = 0;
    char *endptr;
    long double x = strtold(str, &endptr);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * string_castd
 * @brief Parse the string as a double.
 */
double string_castd (const char *str)
{
    errno = 0;
    char *endptr;
    double x = strtod(str, &endptr);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * string_castf
 * @brief Parse the string as a float.
 */
float string_castf (const char *str)
{
    errno = 0;
    char *endptr;
    float x = strtof(str, &endptr);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * string_castll
 * @brief Parse the string as a long long integer.
 */
long long string_castll (const char *str)
{
    errno = 0;
    char *endptr;
    long long x = strtoll(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * string_castl
 * @brief Parse the string as a long integer.
 */
long string_castl (const char *str)
{
    errno = 0;
    char *endptr;
    long x = strtol(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * string_casti
 * @brief Parse the string as an integer.
 */
int string_casti (const char *str)
{
    errno = 0;
    char *endptr;
    long x = strtol(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");

#if (LONG_MIN < INT_MIN || LONG_MAX > INT_MAX)
    /*
     * Ensure 'long' value is within 'int' range
     */
    if (x < INT_MIN || x > INT_MAX) {
        errno = ERANGE;
        core_throw_("range error");
    }
#endif

    return (int) x;
}

/**
 * string_casts
 * @brief Parse the string as a short integer.
 */
short string_casts (const char *str)
{
    errno = 0;
    char *endptr;
    long x = strtol(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");

#if (LONG_MIN < SHRT_MIN || LONG_MAX > SHRT_MAX)
    /*
     * Ensure 'long' value is within 'short' range
     */
    if (x < SHRT_MIN || x > SHRT_MAX) {
        errno = ERANGE;
        core_throw_("range error");
    }
#endif

    return (short) x;
}

/**
 * string_castull
 * @brief Parse the string as a unsigned long long integer.
 */
unsigned long long string_castull (const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long long x = strtoull(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * string_castul
 * @brief Parse the string as a unsigned long integer.
 */
unsigned long string_castul (const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long x = strtoul(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");
    return x;
}

/**
 * string_castui
 * @brief Parse the string as a usigned integer.
 */
unsigned int string_castui (const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long x = strtoul(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");

#if (ULONG_MAX > UINT_MAX)
    /*
     * Ensure 'unsigned long' value is within 'unsigned int' range
     */
    if (x > UINT_MAX) {
        errno = ERANGE;
        core_throw_("range error");
    }
#endif

    return (unsigned int) x;
}

/**
 * string_castus
 * @brief Parse the string as a unsigned sort integer.
 */
unsigned short string_castus (const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long x = strtoul(str, &endptr, 0);

    core_assert_(errno != ERANGE &&
                 *endptr == '\0' &&
                 str != endptr,
                 "invalid number");

#if (ULONG_MAX > USHRT_MAX)
    /*
     * Ensure 'unsigned long' value is within 'unsigned int' range
     */
    if (x > USHRT_MAX) {
        errno = ERANGE;
        core_throw_("range error");
    }
#endif

    return (unsigned short) x;
}
