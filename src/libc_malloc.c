#include "libc_malloc.h"

#include <dlfcn.h>

extern void *__libc_malloc (size_t size);
extern void *__libc_free (void *ptr);

void *libc_malloc (size_t size) {
  return __libc_malloc(size);
}

void libc_free (void *ptr) {
  __libc_free(ptr);
}

int libc_execve (const char *filename, char *const argv[], char *const envp[]) {
  int (*real_execve) (const char *, char *const [], char *const []);
  real_execve = dlsym(RTLD_NEXT, "execve");
  return real_execve(filename, argv, envp);
}
