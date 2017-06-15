#include "libdheap.h"
#include "backtrace.h"
#include "libc_malloc.h"

#include <stdio.h>

void *malloc (size_t size) {
  printf("Malloc called\n");
  return _libc_malloc(size);
}

void free (void *ptr) {
  printf("Free called\n");
  _libc_free(ptr);
}
