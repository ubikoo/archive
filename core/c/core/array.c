/*
 * array.c
 *
  * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "array.h"

/** ---------------------------------------------------------------------------
 * Array function definitions
 */
ARRAY_DEFS(LongDouble, ldouble, long double)
ARRAY_DEFS(Double, double, double)
ARRAY_DEFS(Float, float, float)

ARRAY_DEFS(LongLong, llong, long long)
ARRAY_DEFS(Long, long, long)
ARRAY_DEFS(Int, int, int)
ARRAY_DEFS(Short, short, short)

ARRAY_DEFS(ULongLong, ullong, unsigned long long)
ARRAY_DEFS(ULong, ulong, unsigned long)
ARRAY_DEFS(UInt, uint, unsigned int)
ARRAY_DEFS(UShort, ushort, unsigned short)

ARRAY_DEFS(Char, char, char)
ARRAY_DEFS(UChar, uchar, unsigned char)

ARRAY_DEFS(Int8, int8,  int8_t)
ARRAY_DEFS(Int16, int16, int16_t)
ARRAY_DEFS(Int32, int32, int32_t)
ARRAY_DEFS(Int64, int64, int64_t)

ARRAY_DEFS(UInt8, uint8,  uint8_t)
ARRAY_DEFS(UInt16, uint16, uint16_t)
ARRAY_DEFS(UInt32, uint32, uint32_t)
ARRAY_DEFS(UInt64, uint64, uint64_t)

 ARRAY_DEFS(Void, void, void *)
