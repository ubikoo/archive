/*
 * string.h
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_STRING_H_
#define CORE_STRING_H_

#include "base.h"

/** ---- String array implementation ------------------------------------------
 *
 * struct String
 *
 * @brief String is an array mantaining a null terminated sequence of
 * characters. Unlike a char array, string guarantees the underlying
 * array to be null terminated.
 *
 * @note The string API supports back insertion (removal) of items with
 * constant O(1) complexity.
 *
 *  API array:
 *      array *create_array(capacity)       create an array with capacity>0
 *      void destroy_array(array)           destroy the array and all items
 *
 *      bool array_is_empty(array)          is the array empty?
 *      void array_clear(array)             remove all items from the array
 *
 *      void array_resize(array, capacity)  resize the array
 *      void array_capacity(array)          max array size
 *
 *      void array_push(array, item)        insert an item at the back
 *      item array_pop(array)               remove an item from the back
 *
 * @see
 * Algorithms, 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
struct String {
    size_t capacity;
    size_t size;
    char *data;
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
/** ---- String array implementation ------------------------------------------
 * create_string
 * @brief Create an empty string.
 */
struct String *create_string (const char *source);
/**
 * destroy_string
 * @brief Destroy the string and all items.
 */
void destroy_string (struct String *string);
/**
 * string_is_empty
 * @brief Is the string empty?
 */
bool string_is_empty (const struct String *string);
/**
 * string_clear
 * @brief Remove all items from the string.
 */
void string_clear (struct String *string);
/**
 * string_resize
 * @brief Resize the string.
 */
void string_resize (struct String *string, size_t capacity);
/**
 * string_push
 * @brief Insert an item at the back.
 */
void string_push (struct String *string, char item);
/**
 * string_pop
 * @brief Remove an item from the back.
 */
char string_pop (struct String *string);
/**
 * string_c_copy
 * @brief Copies the c-string pointed to by source to the string.
 */
void string_c_copy (struct String *string, const char *source);
/**
 * string_copy
 * @brief Copies the string pointed to by source to the string.
 */
void string_copy (struct String *string, const struct String *source);
/**
 * string_c_append
 * @brief Appends a c-string pointed to by source to the string.
 */
void string_c_append (struct String *string, const char *source);
/**
 * string_append
 * @brief Appends a string pointed to by source to the string.
 */
void string_append (struct String *string, const struct String *source);
/**
 * string_clone
 * @brief Return a clone of the string.
 */
struct String *string_clone (const struct String *string);


/** ---- String manipulation functions -------------------------------------- */
char *string_ltrim (char * str);
char *string_rtrim (char * str);
char *string_trim (char *str);
char *string_replace (char *str, const char search, const char replace);
char *string_uncomment (char *str, const char end);
char *string_numcpy (char *str, const char *word, long num);

/** ---- String token functions --------------------------------------------- */
char *string_next_token (const char *str);
size_t string_count_tokens (char *str);
char **string_split_tokens (char *str, size_t *n_tokens);
struct String **split_tokens (char *str, size_t *n_tokens);

/** ---- String cast conversion functions ----------------------------------- */
long double string_castld (const char *str);
double string_castd (const char *str);
float string_castf (const char *str);
long long string_castll (const char *str);
long string_castl (const char *str);
int string_casti (const char *str);
short string_casts (const char *str);
unsigned long long string_castull (const char *str);
unsigned long string_castul (const char *str);
unsigned int string_castui (const char *str);
unsigned short string_castus (const char *str);
__END_DECLS

#endif /* CORE_STRING_H_ */
