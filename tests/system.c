/**
 * Test file
 * Executes a vulnerable double free program using system
 */

#include <stdlib.h>

#define VULN_PROG "./double_free"

int main() {
  system(VULN_PROG);
  return 0;
}
