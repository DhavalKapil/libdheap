/**
 * This implements an interface for the original memory allocator functions.
 * Internally, it uses __libc_malloc and __libc_free. 'dylsm' causes issues
 * because it internally uses calloc.
 */

#ifndef _LIBC_MALLOC_GUARD_H
#define _LIBC_MALLOC_GUARD_H

#define _GNU_SOURCE

#include <stddef.h>

void *libc_malloc (size_t size);

void libc_free (void *ptr);

#endif
