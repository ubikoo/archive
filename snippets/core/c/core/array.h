/*
 * array.h
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_ARRAY_H_
#define CORE_ARRAY_H_

#include "base.h"

/** ---- Array implementation -------------------------------------------------
 *
 * struct array_<type>
 *
 * @brief array_<type> represents a finite array of N(>=0) items stored
 * in a contiguous block of memory over which it can iterate.
 *
 * The struct API supports back insertion (and removal) of items
 * with constant O(1) complexity. Front insertion (and removal) of items
 * is not supported.
 * Insertion (and removal) of items from the front of the array has O(N)
 * complexity, because it would involve a displacement (and resizing)
 * of the array to accomodate the new item.
 *
 * @note Unlike a C++ std::vector, the array struct only stores items
 * with a fundamental data <type> that can be trivialliy copied:
 *  char, unsigned char,
 *  short, unsigned short,
 *  int, unsigned int,
 *  long, unsigned long,
 *  long long, unsigned long long,
 *  float, double, long double.
 *
 * Arbitrary objects are stored in an array of pointers to 'void', as
 * represented by the struct array_void. The API provides methods for
 * insertion, removal and retrieval of these pointer values but it does
 * *not* own the data referenced by the pointers.
 * In particular, it is not responsible for the creation, destruction
 * and copy of any items. It is only an aggregate of pointers.
 *
 * @note Unlike its C++ counterpart, the C implementation does not have
 * accessors to the array size, capacity and underlying data structure.
 * Structs in C are data aggregates without encapsulation and for the
 * sake of simplicity, array size, capacity and data are accessible
 * directly from the Array struct.
 *
 *  API array_<type>:
 *      array_<type> *create_array_<type>()         create an empty array
 *      void destroy_array_<type>(array)            destroy the array
 *
 *      bool array_<type>_is_empty(array)           is the array empty?
 *      void array_<type>_clear(array)              remove all items
 *
 *      void array_<type>_resize(array, capacity)   resize the array
 *      void array_<type>_capacity(array)           max array size
 *
 *      void array_<type>_push(array, item)         insert an item at the back
 *      item array_<type>_pop(array)                remove an item from the back
 *
 * @see
 * Algorithms, 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
#include "array.inc"

ARRAY_STRUCT(LongDouble, ldouble, long double)
ARRAY_STRUCT(Double, double, double)
ARRAY_STRUCT(Float, float, float)

ARRAY_STRUCT(LongLong, llong, long long)
ARRAY_STRUCT(Long, long, long)
ARRAY_STRUCT(Int, int, int)
ARRAY_STRUCT(Short, short, short)

ARRAY_STRUCT(ULongLong, ullong, unsigned long long)
ARRAY_STRUCT(ULong, ulong, unsigned long)
ARRAY_STRUCT(UInt, uint, unsigned int)
ARRAY_STRUCT(UShort, ushort, unsigned short)

ARRAY_STRUCT(Char, char, char)
ARRAY_STRUCT(UChar, uchar, unsigned char)

ARRAY_STRUCT(Int8, int8,  int8_t)
ARRAY_STRUCT(Int16, int16, int16_t)
ARRAY_STRUCT(Int32, int32, int32_t)
ARRAY_STRUCT(Int64, int64, int64_t)

ARRAY_STRUCT(UInt8, uint8,  uint8_t)
ARRAY_STRUCT(UInt16, uint16, uint16_t)
ARRAY_STRUCT(UInt32, uint32, uint32_t)
ARRAY_STRUCT(UInt64, uint64, uint64_t)

ARRAY_STRUCT(Void, void, void *)

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
/** ---------------------------------------------------------------------------
 * Array function declarations.
 */
ARRAY_DECLS(LongDouble, ldouble, long double)
ARRAY_DECLS(Double, double, double)
ARRAY_DECLS(Float, float, float)

ARRAY_DECLS(LongLong, llong, long long)
ARRAY_DECLS(Long, long, long)
ARRAY_DECLS(Int, int, int)
ARRAY_DECLS(Short, short, short)

ARRAY_DECLS(ULongLong, ullong, unsigned long long)
ARRAY_DECLS(ULong, ulong, unsigned long)
ARRAY_DECLS(UInt, uint, unsigned int)
ARRAY_DECLS(UShort, ushort, unsigned short)

ARRAY_DECLS(Char, char, char)
ARRAY_DECLS(UChar, uchar, unsigned char)

ARRAY_DECLS(Int8, int8,  int8_t)
ARRAY_DECLS(Int16, int16, int16_t)
ARRAY_DECLS(Int32, int32, int32_t)
ARRAY_DECLS(Int64, int64, int64_t)

ARRAY_DECLS(UInt8, uint8,  uint8_t)
ARRAY_DECLS(UInt16, uint16, uint16_t)
ARRAY_DECLS(UInt32, uint32, uint32_t)
ARRAY_DECLS(UInt64, uint64, uint64_t)

ARRAY_DECLS(Void, void, void *)
__END_DECLS

#endif /* CORE_ARRAY_H_ */
