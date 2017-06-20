#ifndef _LIBDHEAP_GUARD_H
#define _LIBDHEAP_GUARD_H

#include <stddef.h>

/**
 * Initializer function
 */
void initialize (void);

// Redefining wrappers over memory management functions
// ANSI functions
void *malloc (size_t size);
void free (void *ptr);
void *calloc (size_t nmemb, size_t size);
void *realloc (void *ptr, size_t size);

// Redifining additional functions
size_t malloc_usable_size (void *ptr);

#endif
