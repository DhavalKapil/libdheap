/**
 * This implements an interface for the original memory allocator functions.
 */

#ifndef _LIBC_MALLOC_GUARD_H
#define _LIBC_MALLOC_GUARD_H

#define _GNU_SOURCE

#include <dlfcn.h>

// The libc allocator functions
void *(*_libc_malloc) (size_t);
void (*_libc_free) (void *);

/**
 * Constructor function, called when library is loaded
 */
static void init() __attribute__ ((constructor));

void init() {
  // Dynamically loading them directly
  _libc_malloc = dlsym(RTLD_NEXT, "malloc");
  _libc_free = dlsym(RTLD_NEXT, "free");
}

#endif
