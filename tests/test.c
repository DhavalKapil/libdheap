#include <stdlib.h>
#include <stdio.h>

int main() {
  void *ptr[1000];
  int i = 0;
  for (;i<1000;i++) {
    ptr[i] = malloc(10);
  }
  for(i=0;i<500;i++) {
    free(ptr[i]);
  }
  for(i=0;i<500;i++) {
    ptr[i] = malloc(20);
  }
  for(i=0;i<1000;i++) {
    free(ptr[i]);
  }
  return 0;
}
