/**
 * This implements an interface for the original memory allocator functions.
 * Internally, it uses __libc_malloc and __libc_free. 'dylsm' causes issues
 * because it internally uses calloc.
 *
 * This also implements an interface for execve to propagate libdheap
 * environment variable further.
 */

#ifndef _LIBC_MALLOC_GUARD_H
#define _LIBC_MALLOC_GUARD_H

#define _GNU_SOURCE

#include <stddef.h>

void *libc_malloc (size_t size);

void libc_free (void *ptr);

int libc_execve (const char *filename, char *const argv[], char *const envp[]);

#endif
