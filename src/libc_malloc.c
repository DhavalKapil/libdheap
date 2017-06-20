#include "libc_malloc.h"

extern void *__libc_malloc (size_t size);
extern void *__libc_free (void *ptr);

void *libc_malloc (size_t size) {
  return __libc_malloc(size);
}

void libc_free (void *ptr) {
  __libc_free(ptr);
}
