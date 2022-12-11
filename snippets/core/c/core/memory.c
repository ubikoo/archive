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

#include "memory.h"

/** ---- Memory aligned allocator ---------------------------------------------
 *
 * align_alloc
 *
 * @brief Allocate a memory block with size bytes on a boundary specified
 * by the alignment CORE_L1_CACHE_LINE_SIZE.
 *
 * @throw
 *  If block size is invalid.
 *  If alignment is not a power of 2.
 *  If posix_memalign fails to allocate.
 *
 * @note
 *  The alignment must be a power of two and a multiple of sizeof(void *).
 *  Given n > 1:
 *
 *  If n is a power of 2, then n takes the form, e.g., 01000000.
 *  Subtract 1 from n,
 *      (n-1) = 01000000 - 00000001 = 00111111.
 *  Bitwise n & (n-1) == 0
 *      (00111111) & (01000000) = (00000000).
 *
 *  Any n not a power of 2, will have an extra 1, e.g., 01000010.
 *  Subtract 1 from n,
 *      (n-1) = 01000010 - 00000001 = 01000001.
 *  Bitwise n & (n-1) != 0
 *      (01000001) & (01000010) = (01000000).
 *
 * @note
 * align_alloc and align_free are paired functions: an object created
 * with align_alloc should be freed with align_free.
 */
void *align_alloc (size_t size)
{
    const size_t alignment = CORE_L1_CACHE_LINE_SIZE;

    /*
     * Check size is valid and alignment is power of 2.
     */
    core_assert_(size > 0, "invalid size");
    core_assert_(alignment > 0 && !((alignment-1) & alignment),
                 "alignment value is not a power of 2");

    /*
     * Allocate block and check for errors.
     */
    void *ptr = NULL;
    int ret = posix_memalign(&ptr, alignment, size);

    core_assert_(
        ret != EINVAL,
        "alignment value is not a power of 2 nor multiple of sizeof(void *)");

    core_assert_(
        ret != ENOMEM,
        "insufficient memory available with the requested alignment");

    core_assert_(ptr != NULL, "failed to allocate");

    /*
     * Ensure the memory block is initialized before returning the ptr.
     */
    memset(ptr, 0, size);
    return ptr;
}


/**
 * align_free
 *
 * @brief Free a memory block.
 *
 * @note
 *  Ensure ptr is set to null after deallocation.
 *  If ptr is not null and refers to a space that has been deallocated
 *  by a call to align_free or align_realloc, the result of align_free
 *  is undefined.
 *
 * @note
 * align_alloc and align_free are paired functions: an object created
 * with align_alloc should be freed with align_free.
 */
void align_free (void *ptr)
{
    if (ptr == NULL) {
        return;
    }
    free(ptr);
}


/**
 * align_realloc
 *
 * @brief Reallocate an array of size (oldsize = oldelms*size) bytes to a
 * new array of size (newsize = newelms*size) bytes on a boundary specified
 * by the default alignment set by the L1 cache line size.
 *
 * The address of a block returned by malloc or realloc in 32-bit
 * systems is always a multiple of eight (or a multiple of sixteen
 * on 64-bit systems).
 *
 * The current default L1 cache line size is 32-bit. This function
 * reimplements the realloc specification to the new boundary size.
 *
 * Summary of the relevant standard requirements for realloc:
 *
 * - The contents of the object shall remain unchanged up to the lesser
 *   of the new and old sizes. If the new block size requires movement
 *   of the object, the space for the previous instantiation of the
 *   object is freed.
 *
 * - If the new block size is smaller (larger) than the old block size,
 *   align_realloc will allocate a new block of the appriate size,
 *   copy the contents of the original block up to the new (old) size,
 *   free the original block and return a pointer to the newly created
 *   block.
 *   If the new block size is larger, the contents of the newly allocated
 *   portion are initialised to 0 as set by align_alloc.
 *
 * - If the input pointer is null, align_realloc behaves exactly as if
 *   align_alloc has been called.
 *
 * - If the newsize is 0 and ptr is not a null pointer, align_realloc
 *   behaves exactly as if align_free has been called and return a null
 *   value.
 *
 * - If the space cannot be allocated, the object shall remain unchanged.
 *   If the input pointer is null and the requested size is 0, then the
 *   result is undefined.
 */
void *align_realloc (void *ptr, size_t oldsize, size_t newsize)
{
    /*
     * New size is 0 - free the ptr and return null. Align free
     * a null pointer is a noop, otherwise the memory is freed.
     */
    if (newsize == 0) {
        align_free(ptr);
        return NULL;
    }

    /* If ptr is a null pointer, return a newly allocated block */
    if (ptr == NULL) {
        return align_alloc(newsize);
    }

    /*
     * Otherwise, create a new block and copy the contents of the
     * original block up to the lesser of the new and old sizes.
     */
    size_t size = newsize > oldsize ? oldsize : newsize;
    void *mem = align_alloc(newsize);
    memcpy(mem, ptr, size);

    /*
     * Free the original block and return the newly created block.
     */
    align_free(ptr);
    return mem;
}
