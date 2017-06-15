#include <stdlib.h>

int main() {
  void *ptr;
  int i = 0;
  for (;i<10;i++) {
    ptr = malloc(10);
    free(ptr);
  }
  return 0;
}
