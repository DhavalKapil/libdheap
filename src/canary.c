#include "canary.h"

#include <fcntl.h>
#include <unistd.h>

/**
 * Returns a pointer to the begin canary of a chunk
 *
 * @param ch The required chunk
 *
 * @return The canary *
 */
inline canary *get_begin_canary (struct chunk *ch) {
  return (canary *)((char *)ch->ptr + sizeof(canary));
}

/**
 * Returns a pointer to the end canary of a chunk
 *
 * @param ch The required chunk
 *
 * @return The canary *
 */
inline canary *get_end_canary (struct chunk *ch) {
  return (canary *)((char *)ch->ptr +
                    ch->requested_size -
                    sizeof(canary));
}

// Implementing functions defined in header file

size_t get_padded_size (size_t size) {
  return (size + 2*sizeof(canary));
}


canary generate_canary () {
  canary c;
  size_t size;
  unsigned char *p;
  int fd;

  fd = open("/dev/urandom", O_RDONLY);
  if (fd >= 0) {
    size = read(fd, &c, sizeof(canary));
    close(fd);
    if (size == sizeof(canary)) {
      return c;
    }
  }
  // Some error
  // Create a canary difficult to overflow
  c = 0;
  p = (unsigned char *)&c;
  p[sizeof(c) - 1] = '\n';
  p[sizeof(c) - 2] = '\0';
  return c;
}

void guard_chunk (struct chunk *ch) {
  ch->begin_guard = generate_canary();
  ch->end_guard = generate_canary();
  // Now putting guards inside chunk
  *get_begin_canary(ch) = ch->begin_guard;
  *get_end_canary(ch) = ch->end_guard;
}

int check_canary (struct chunk *ch) {
  if (ch->begin_guard != *get_begin_canary(ch)) {
    return 0;
  } else if (ch->end_guard != *get_end_canary(ch)) {
    return 0;
  }
  return 1;
}
