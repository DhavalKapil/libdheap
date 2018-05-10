/**
 * Test file
 * Executes a vulnerable double free program using execve
 */

#include <unistd.h>

#define VULN_PROG "./double-free"

int main() {
  char *argv[2];
  argv[0] = VULN_PROG;
  argv[1] = 0;
  execve(VULN_PROG, argv, 0);
  return 0;
}
