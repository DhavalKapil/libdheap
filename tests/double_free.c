/**
 * Test file
 * Tries to attempt a double free
 */

#include <stdlib.h>

int main() {
  void *ptr[100];
  int i;

  for (i = 0;i<100;i++) {
    ptr[i] = malloc(45);
  }

  for (i = 0;i<100;i++) {
    free(ptr[i]);
  }

  free(ptr[56]);
  return 0;
}
