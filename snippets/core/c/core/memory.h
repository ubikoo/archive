/*
 * memory.c
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef CORE_MEMORY_H_
#define CORE_MEMORY_H_

#include "base.h"

/*
 * Core default cache line size
 */
#ifndef CORE_L1_CACHE_LINE_SIZE
#define CORE_L1_CACHE_LINE_SIZE 32
#endif

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
/** ---- Memory aligned allocator ------------------------------------------- */
void *align_alloc (size_t size);
void align_free (void *ptr);
void *align_realloc (void *ptr, size_t oldsize, size_t newsize);
__END_DECLS

#endif /* CORE_MEMORY_H_ */
