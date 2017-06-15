#define _GNU_SOURCE

#include "libdheap.h"
#include "backtrace.h"

#include <stdio.h>
#include <dlfcn.h>

void *malloc (size_t size) {
  void *(*o_malloc) (size_t);
  printf("Malloc called\n");
  o_malloc = dlsym(RTLD_NEXT, "malloc");
  return o_malloc(size);
}

void free (void *ptr) {
  void *(*o_free) (void *);
  printf("Free called\n");
  o_free = dlsym(RTLD_NEXT, "free");
  o_free(ptr);
}
