#include <stdlib.h>
#include <string.h>

/**
 * Returns LIBDHEAP_DEBUG
 */
int is_libdheap_debug (void) {
  char *debug;
  debug = getenv("LIBDHEAP_DEBUG");
  if (debug && !strcmp(debug, "1")) {
    return 1;
  } else {
    return 0;
  }
}

/**
 * Returns LIBDHEAP_EXIT_ON_ERROR
 */
int is_libdheap_exit_on_error (void) {
  char *exit_on_error;
  exit_on_error = getenv("LIBDHEAP_EXIT_ON_ERROR");
  if (exit_on_error && !strcmp(exit_on_error, "1")) {
    return 1;
  } else {
    return 0;
  }
}
