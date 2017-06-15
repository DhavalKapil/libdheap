#ifndef _LIBDHEAP_GUARD_H
#define _LIBDHEAP_GUARD_H

#include <stddef.h>

// Redefining wrappers over memory management functions
void *malloc (size_t size);
void free (void *ptr);

#endif
