#include "display.h"
#include "config.h"

#include <stdio.h>
#include <stdarg.h>

void display_error (const char *msg, ...) {
  va_list argptr;
  va_start(argptr, msg);
  fprintf(stderr, "[LIBDHEAP ERROR] : ");
  vfprintf(stderr, msg, argptr);
  va_end(argptr);
  fprintf(stderr, "\n");
}

void display_log (FILE *stream, const char *log, ...) {
  va_list argptr;
  va_start(argptr, log);
  fprintf(stream, "[LIBDHEAP LOG] : ");
  vfprintf(stream, log, argptr);
  va_end(argptr);
  fprintf(stream, "\n");
}

void display_debug (FILE *stream, const char *msg, ...) {
  va_list argptr;

  if (!is_libdheap_debug())
    return;

  va_start(argptr, msg);
  fprintf(stream, "[LIBDHEAP DEBUG] : ");
  vfprintf(stream, msg, argptr);
  va_end(argptr);
  fprintf(stream, "\n");
}
