/**
 * Test file
 * Attempts buffer overflow
 */

#include <stdlib.h>
#include <string.h>

int main() {
  char *ptr = malloc(10);
  strcpy(ptr, "A big string");
  free(ptr);
  return 0;
}
