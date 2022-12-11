/*
 * array.inc
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

/** ---------------------------------------------------------------------------
 * Array structure definition.
 */
#ifndef ARRAY_STRUCT
#define ARRAY_STRUCT(Name,Suffix,Type) \
struct Array##Name { \
    size_t capacity; \
    size_t size; \
    Type *data; \
};
#endif /* ARRAY_STRUCT */

/** ---------------------------------------------------------------------------
 * Array function declarations.
 */
#ifndef ARRAY_DECLS
#define ARRAY_DECLS(Name,Suffix,Type) \
/** \
 * create_array \
 * @brief Create an array with n>=0 items. \
 */ \
struct Array##Name *create_array_##Suffix (void); \
/** \
 * destroy_array \
 * @brief Destroy the array and all items. \
 */ \
void destroy_array_##Suffix (struct Array##Name *array); \
/** \
 * array_is_empty \
 * @brief Is the array empty? \
 */ \
bool array_##Suffix##_is_empty (const struct Array##Name *array); \
/** \
 * array_clear \
 * @brief Remove all items from the array. \
 */ \
void array_##Suffix##_clear (struct Array##Name *array); \
/** \
 * array_resize \
 * @brief Resize the array. \
 */ \
void array_##Suffix##_resize (struct Array##Name *array, size_t capacity); \
/** \
 * array_push \
 * @brief Insert an item at the back. \
 */ \
void array_##Suffix##_push (struct Array##Name *array, Type item); \
/** \
 * array_pop \
 * @brief Remove an item from the back. \
 */ \
Type array_##Suffix##_pop (struct Array##Name *array);
#endif /* ARRAY_DECLS */


/** ---------------------------------------------------------------------------
 * Array function definitions.
 */
#ifndef ARRAY_DEFS
#define ARRAY_DEFS(Name,Suffix,Type) \
/** \
 * create_array_type \
 * @brief Create an array with n>=0 items. \
 */ \
struct Array##Name *create_array_##Suffix (void) \
{ \
    struct Array##Name *array = \
        (struct Array##Name *) calloc(1, sizeof(struct Array##Name)); \
    core_assert_(array != NULL, "failed to alloc array\n"); \
    array_##Suffix##_clear(array); \
    return array; \
} \
\
/** \
 * destroy_array_type \
 * @brief Destroy the array and all items. \
 */ \
void destroy_array_##Suffix (struct Array##Name *array) \
{ \
    core_assert_(array != NULL, "invalid array"); \
    free(array->data); \
    free(array); \
} \
\
/** \
 * array_type_is_empty \
 * @brief Is the array empty? \
 */ \
bool array_##Suffix##_is_empty (const struct Array##Name *array) \
{ \
    core_assert_(array != NULL, "invalid array"); \
    return (array->size == 0); \
} \
\
/** \
 * array_type_clear \
 * @brief Remove all items from the array. \
 */ \
void array_##Suffix##_clear (struct Array##Name *array) \
{ \
    core_assert_(array != NULL, "invalid array"); \
\
    free(array->data); \
    array->data = (Type *) calloc(1, sizeof(Type)); \
    core_assert_(array->data != NULL, "failed to alloc array->data\n"); \
    array->capacity = 1; \
    array->size = 0; \
} \
\
/** \
 * array_type_resize \
 * @brief Resize the array. \
 */ \
void array_##Suffix##_resize (struct Array##Name *array, size_t capacity) \
{ \
    core_assert_(array != NULL, "invalid array"); \
    core_assert_(capacity > 0, "invalid size"); \
\
    /* \
     * Create a copy array with the new capacity. \
     */ \
    Type *copy = (Type *) calloc(capacity, sizeof(Type)); \
    core_assert_(copy != NULL, "failed to alloc copy\n"); \
\
    /* \
     * Copy the items of the current array and swap the data. \
     */ \
    array->capacity = capacity; \
    array->size = array->size < capacity ? array->size : capacity; \
    memcpy(copy, array->data, array->size*sizeof(Type)); \
    free(array->data); \
    array->data = copy; \
} \
\
/** \
 * array_type_push \
 * @brief Insert an item at the back. \
 */ \
void array_##Suffix##_push (struct Array##Name *array, Type item) \
{ \
    core_assert_(array != NULL, "invalid array"); \
    if (array->size == array->capacity) { \
        array_##Suffix##_resize(array, 2 * array->capacity); \
    } \
    array->data[array->size] = item; \
    array->size++; \
} \
\
/** \
 * array_type_pop \
 * @brief Remove an item from the back. \
 */ \
Type array_##Suffix##_pop (struct Array##Name *array) \
{ \
    core_assert_(array != NULL, "invalid array"); \
    core_assert_( \
        !array_##Suffix##_is_empty(array), \
        "empty array, out of range error"); \
\
    /* \
     * Remove the data item from the array. If the array still \
     * contains items and its size is 1/4 of the the current \
     * capacity, then we can halve the array capacity while \
     * maintaining a 2:1 ratio. \
     */ \
    --array->size; \
    Type item = array->data[array->size]; \
    if (array->size > 0 && array->size == (array->capacity / 4)) { \
        array_##Suffix##_resize(array, array->capacity / 2); \
    } \
    return item; \
}
#endif /* ARRAY_DEFS */
