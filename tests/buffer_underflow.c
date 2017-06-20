/**
 * Test file
 * Attempts buffer underflow
 */

#include <stdlib.h>
#include <stddef.h>

struct chunk_struct {
  size_t prev_size;
  size_t size;
  char buf[10];
};

int main() {
  struct chunk_struct *fake_chunk;
  size_t *ptr = malloc(100);
  ptr[-1] = 0x51;

  // This bypasses "free(): invalid next size (fast)" security check
  // of glibc.
  fake_chunk = (struct chunk_struct *)((char *)(ptr-2) + 0x51);
  fake_chunk->size = 0x51;

  free(ptr);
  return 0;
}
